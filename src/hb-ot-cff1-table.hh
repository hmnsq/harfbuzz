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

#அந்தர்பாவ "hb-ot-cff-common.hh"
#அந்தர்பாவ "hb-depend-data.hh"
#அந்தர்பாவ "hb-subset-cff-common.hh"
#அந்தர்பாவ "hb-draw.hh"
#அந்தர்பாவ "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#அந்தர்பாவ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

நாமாகாஶ CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

எண்ணிக்கை EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
எண்ணிக்கை CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

வகைவரை CFF1Index          CFF1CharStrings;
வகைவரை Subrs<HBUINT16>    CFF1Subrs;

கட்டமைப்பு CFF1FDSelect : FDSelect {};

/* Encoding */
கட்டமைப்பு Encoding0 {
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) மாறிலி
  {
    ப்ரதிஜ்ஞா (glyph > 0);
    glyph--;
    எனில் (glyph < nCodes ())
    {
      திரும்பு (hb_codepoint_t)codes[glyph];
    }
    இல்லையெனில்
      திரும்பு CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { திரும்பு codes.len; }
  HBUINT8 nCodes () மாறிலி { திரும்பு codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

கட்டமைப்பு Encoding1_Range {
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT8   first;
  HBUINT8   n_இடது;

  DEFINE_SIZE_STATIC (2);
};

கட்டமைப்பு Encoding1 {
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) மாறிலி
  {
    /* TODO: Add cache like get_sid. */
    ப்ரதிஜ்ஞா (glyph > 0);
    glyph--;
    க்கு (unsigned முழு i = 0; i < nRanges (); i++)
    {
      எனில் (glyph <= ranges[i].n_இடது)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	திரும்பு (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_இடது + 1);
    }
    திரும்பு CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { திரும்பு ranges.len; }
  HBUINT8 nRanges () மாறிலி { திரும்பு ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

கட்டமைப்பு SuppEncoding {
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

கட்டமைப்பு CFF1SuppEncData {
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (supps.sanitize (c));
  }

  வெறுமை get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) மாறிலி
  {
    க்கு (unsigned முழு i = 0; i < nSups (); i++)
      எனில் (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { திரும்பு supps.len; }
  HBUINT8 nSups () மாறிலி { திரும்பு supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

கட்டமைப்பு Encoding
{
  /* serialize a fullset Encoding */
  தருக்க serialize (hb_serialize_context_t *c, மாறிலி Encoding &src)
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  தருக்க serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned முழு enc_மொத்தம்,
		  மாறிலி hb_vector_t<code_pair_t>& code_ranges,
		  மாறிலி hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (இது);
    Encoding *dest = c->extend_min (இது);
    எனில் (unlikely (!dest)) return_trace (பொய்);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    தேர்வு (format) {
    நிலை 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_மொத்தம்);
      எனில் (unlikely (!fmt0)) return_trace (பொய்);
      fmt0->nCodes () = enc_மொத்தம்;
      unsigned முழு glyph = 0;
      க்கு (unsigned முழு i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	க்கு (முழு left = (முழு)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	எனில் (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (பொய்);
      }
    }
    நிறுத்து;

    நிலை 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      எனில் (unlikely (!fmt1)) return_trace (பொய்);
      fmt1->nRanges () = code_ranges.length;
      க்கு (unsigned முழு i = 0; i < code_ranges.length; i++)
      {
	எனில் (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (பொய்);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_இடது = code_ranges[i].glyph;
      }
    }
    நிறுத்து;

    }

    எனில் (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      எனில் (unlikely (!suppData)) return_trace (பொய்);
      suppData->nSups () = supp_codes.length;
      க்கு (unsigned முழு i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (உண்மை);
  }

  size_t get_size () மாறிலி
  {
    size_t size = min_size;
    தேர்வு (table_format ())
    {
    நிலை 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); நிறுத்து;
    நிலை 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); நிறுத்து;
    }
    எனில் (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    திரும்பு size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) மாறிலி
  {
    தேர்வு (table_format ())
    {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_code (glyph);
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_code (glyph);
    இயல்பு:திரும்பு 0;
    }
  }

  uint8_t table_format () மாறிலி { திரும்பு format & 0x7F; }
  தருக்க  has_supplement () மாறிலி { திரும்பு format & 0x80; }

  வெறுமை get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) மாறிலி
  {
    codes.clear ();
    எனில் (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();

    தேர்வு (table_format ())
    {
    நிலை 0: hb_barrier (); எனில் (unlikely (!u.format0.sanitize (c))) { return_trace (பொய்); } நிறுத்து;
    நிலை 1: hb_barrier (); எனில் (unlikely (!u.format1.sanitize (c))) { return_trace (பொய்); } நிறுத்து;
    இயல்பு:return_trace (பொய்);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  பாதுகாக்கப்பட்ட:
  மாறிலி CFF1SuppEncData &suppEncData () மாறிலி
  {
    தேர்வு (table_format ())
    {
    நிலை 0: hb_barrier (); திரும்பு StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    நிலை 1: hb_barrier (); திரும்பு StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    இயல்பு:திரும்பு Null (CFF1SuppEncData);
    }
  }

  பொது:
  HBUINT8	format;
  ஒன்றிப்பு {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
கட்டமைப்பு Charset0
{
  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு இலக்கம்_glyphs, unsigned *இலக்கம்_charset_entries) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (இலக்கம்_charset_entries) *இலக்கம்_charset_entries = இலக்கம்_glyphs;
    return_trace (sids.sanitize (c, இலக்கம்_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned இலக்கம்_glyphs) மாறிலி
  {
    எனில் (unlikely (glyph >= இலக்கம்_glyphs)) திரும்பு 0;
    எனில் (unlikely (glyph == 0))
      திரும்பு 0;
    இல்லையெனில்
      திரும்பு sids[glyph - 1];
  }

  வெறுமை collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    எனில் (!mapping->resize_dirty  (இலக்கம்_glyphs)) திரும்பு;
    க்கு (hb_codepoint_t gid = 1; gid < இலக்கம்_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    எனில் (sid == 0)
      திரும்பு 0;

    க்கு (unsigned முழு glyph = 1; glyph < இலக்கம்_glyphs; glyph++)
    {
      எனில் (sids[glyph-1] == sid)
	திரும்பு glyph;
    }
    திரும்பு 0;
  }

  நிலையான size_t get_size (unsigned முழு இலக்கம்_glyphs)
  {
    ப்ரதிஜ்ஞா (இலக்கம்_glyphs > 0);
    திரும்பு UnsizedArrayOf<HBUINT16>::get_size (இலக்கம்_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
கட்டமைப்பு Charset_Range {
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT16  first;
  TYPE      n_இடது;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
கட்டமைப்பு Charset1_2 {
  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு இலக்கம்_glyphs, unsigned *இலக்கம்_charset_entries) மாறிலி
  {
    TRACE_SANITIZE (இது);
    இலக்கம்_glyphs--;
    unsigned i;
    க்கு (i = 0; இலக்கம்_glyphs > 0; i++)
    {
      எனில் (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (இலக்கம்_glyphs >= ranges[i].n_இடது + 1))))
	return_trace (பொய்);
      இலக்கம்_glyphs -= (ranges[i].n_இடது + 1);
    }
    எனில் (இலக்கம்_charset_entries)
      *இலக்கம்_charset_entries = i;
    return_trace (உண்மை);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned இலக்கம்_glyphs,
			  code_pair_t *cache = nullptr) மாறிலி
  {
    எனில் (unlikely (glyph >= இலக்கம்_glyphs)) திரும்பு 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    எனில் (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    இல்லையெனில்
    {
      எனில் (unlikely (glyph == 0)) திரும்பு 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    க்கு (;; i++)
    {
      unsigned மொத்தம் = ranges[i].n_இடது;
      எனில் (glyph <= மொத்தம்)
      {
        எனில் (cache)
	  *cache = {i, start_glyph};
	திரும்பு ranges[i].first + glyph;
      }
      மொத்தம்++;
      start_glyph += மொத்தம்;
      glyph -= மொத்தம்;
    }

    திரும்பு 0;
  }

  வெறுமை collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    எனில் (!mapping->resize_dirty  (இலக்கம்_glyphs)) திரும்பு;

    hb_codepoint_t gid = 1;
    எனில் (gid >= இலக்கம்_glyphs)
      திரும்பு;

    க்கு (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned மொத்தம் = ranges[i].n_இடது + 1;
      unsigned last = gid + மொத்தம்;
      க்கு (unsigned j = 0; j < மொத்தம்; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      எனில் (gid >= இலக்கம்_glyphs)
        நிறுத்து;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    எனில் (sid == 0) திரும்பு 0;
    hb_codepoint_t  glyph = 1;
    க்கு (unsigned முழு i = 0;; i++)
    {
      எனில் (glyph >= இலக்கம்_glyphs)
	திரும்பு 0;
      எனில் ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_இடது))
	திரும்பு glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_இடது + 1);
    }

    திரும்பு 0;
  }

  size_t get_size (unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    முழு glyph = (முழு) இலக்கம்_glyphs;
    unsigned இலக்கம்_ranges = 0;

    ப்ரதிஜ்ஞா (glyph > 0);
    glyph--;
    க்கு (unsigned முழு i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_இடது + 1);
      இலக்கம்_ranges++;
    }

    திரும்பு get_size_for_ranges (இலக்கம்_ranges);
  }

  நிலையான size_t get_size_for_ranges (unsigned முழு இலக்கம்_ranges)
  {
    திரும்பு UnsizedArrayOf<Charset_Range<TYPE> >::get_size (இலக்கம்_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

வகைவரை Charset1_2<HBUINT8>     Charset1;
வகைவரை Charset1_2<HBUINT16>    Charset2;
வகைவரை Charset_Range<HBUINT8>  Charset1_Range;
வகைவரை Charset_Range<HBUINT16> Charset2_Range;

கட்டமைப்பு Charset
{
  /* serialize a fullset Charset */
  தருக்க serialize (hb_serialize_context_t *c, மாறிலி Charset &src, unsigned முழு இலக்கம்_glyphs)
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed ((மாறிலி எழுத்து *) &src, src.get_size (இலக்கம்_glyphs)));
  }

  /* serialize a subset Charset */
  தருக்க serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned முழு இலக்கம்_glyphs,
		  மாறிலி hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (இது);
    Charset *dest = c->extend_min (இது);
    எனில் (unlikely (!dest)) return_trace (பொய்);
    dest->format = format;
    தேர்வு (format)
    {
    நிலை 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (இலக்கம்_glyphs), பொய்);
      எனில் (unlikely (!fmt0)) return_trace (பொய்);
      unsigned முழு glyph = 0;
      க்கு (unsigned முழு i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	க்கு (முழு left = (முழு)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    நிறுத்து;

    நிலை 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), பொய்);
      எனில் (unlikely (!fmt1)) return_trace (பொய்);
      hb_codepoint_t all_glyphs = 0;
      க்கு (unsigned முழு i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_இடது = _.glyph;
      }
      எனில் (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (பொய்);
    }
    நிறுத்து;

    நிலை 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), பொய்);
      எனில் (unlikely (!fmt2)) return_trace (பொய்);
      hb_codepoint_t all_glyphs = 0;
      க்கு (unsigned முழு i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_இடது = _.glyph;
      }
      எனில் (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (பொய்);
    }
    நிறுத்து;

    }
    return_trace (உண்மை);
  }

  size_t get_size (unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு hb_unsigned_add_saturate (min_size, u.format0.get_size (இலக்கம்_glyphs));
    நிலை 1: hb_barrier (); திரும்பு hb_unsigned_add_saturate (min_size, u.format1.get_size (இலக்கம்_glyphs));
    நிலை 2: hb_barrier (); திரும்பு hb_unsigned_add_saturate (min_size, u.format2.get_size (இலக்கம்_glyphs));
    இயல்பு:திரும்பு 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned முழு இலக்கம்_glyphs,
			  code_pair_t *cache = nullptr) மாறிலி
  {
    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_sid (glyph, இலக்கம்_glyphs);
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_sid (glyph, இலக்கம்_glyphs, cache);
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_sid (glyph, இலக்கம்_glyphs, cache);
    இயல்பு:திரும்பு 0;
    }
  }

  வெறுமை collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, இலக்கம்_glyphs); திரும்பு;
    நிலை 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, இலக்கம்_glyphs); திரும்பு;
    நிலை 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, இலக்கம்_glyphs); திரும்பு;
    இயல்பு:திரும்பு;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_glyph (sid, இலக்கம்_glyphs);
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_glyph (sid, இலக்கம்_glyphs);
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_glyph (sid, இலக்கம்_glyphs);
    இயல்பு:திரும்பு 0;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned *இலக்கம்_charset_entries) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();

    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_இலக்கம்_glyphs (), இலக்கம்_charset_entries));
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_இலக்கம்_glyphs (), இலக்கம்_charset_entries));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_இலக்கம்_glyphs (), இலக்கம்_charset_entries));
    இயல்பு:return_trace (பொய்);
    }
  }

  HBUINT8       format;
  ஒன்றிப்பு {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

கட்டமைப்பு CFF1StringIndex : CFF1Index
{
  தருக்க serialize (hb_serialize_context_t *c, மாறிலி CFF1StringIndex &strings,
		  மாறிலி hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely ((strings.மொத்தம் == 0) || (sidmap.length == 0)))
    {
      எனில் (unlikely (!c->extend_min (இது->மொத்தம்)))
	return_trace (பொய்);
      மொத்தம் = 0;
      return_trace (உண்மை);
    }

    எனில் (unlikely (sidmap.in_error ())) return_trace (பொய்);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    எனில் (unlikely (bytesArray.in_error ())) return_trace (பொய்);

    தருக்க result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

கட்டமைப்பு cff1_உச்சி_dict_interp_env_t : இலக்கம்_interp_env_t
{
  cff1_உச்சி_dict_interp_env_t ()
    : இலக்கம்_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_உச்சி_dict_interp_env_t (மாறிலி hb_ubytes_t &bytes)
    : இலக்கம்_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned முழு prev_offset;
  unsigned முழு last_offset;
};

கட்டமைப்பு name_dict_மதிப்புகள்_t
{
  எண்ணிக்கை name_dict_val_index_t
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

      Val_மொத்தம்
  };

  வெறுமை init ()
  {
    க்கு (unsigned முழு i = 0; i < Val_மொத்தம்; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned முழு& operator[] (unsigned முழு i)
  { ப்ரதிஜ்ஞா (i < Val_மொத்தம்); திரும்பு values[i]; }

  unsigned முழு operator[] (unsigned முழு i) மாறிலி
  { ப்ரதிஜ்ஞா (i < Val_மொத்தம்); திரும்பு values[i]; }

  நிலையான எண்ணிக்கை name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    தேர்வு (op) {
      இயல்பு: // can't happen - just make some compiler happy
      நிலை OpCode_version:
	திரும்பு version;
      நிலை OpCode_Notice:
	திரும்பு notice;
      நிலை OpCode_Copyright:
	திரும்பு copyright;
      நிலை OpCode_FullName:
	திரும்பு fullName;
      நிலை OpCode_FamilyName:
	திரும்பு familyName;
      நிலை OpCode_Weight:
	திரும்பு weight;
      நிலை OpCode_PostScript:
	திரும்பு postscript;
      நிலை OpCode_FontName:
	திரும்பு fontName;
      நிலை OpCode_BaseFontName:
	திரும்பு baseFontName;
    }
  }

  unsigned முழு  values[Val_மொத்தம்];
};

கட்டமைப்பு cff1_உச்சி_dict_val_t : op_str_t
{
  unsigned முழு  last_arg_offset;
};

கட்டமைப்பு cff1_உச்சி_dict_மதிப்புகள்_t : உச்சி_dict_மதிப்புகள்_t<cff1_உச்சி_dict_val_t>
{
  வெறுமை init ()
  {
    உச்சி_dict_மதிப்புகள்_t<cff1_உச்சி_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_மொத்தம் = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  வெறுமை fini () { உச்சி_dict_மதிப்புகள்_t<cff1_உச்சி_dict_val_t>::fini (); }

  தருக்க is_CID () மாறிலி
  { திரும்பு nameSIDs[name_dict_மதிப்புகள்_t::registry] != CFF_UNDEF_SID; }

  name_dict_மதிப்புகள்_t  nameSIDs;
  unsigned முழு    ros_supplement_offset;
  unsigned முழு    ros_supplement;
  unsigned முழு    cid_மொத்தம்;

  முழு             EncodingOffset;
  முழு             CharsetOffset;
  முழு             FDSelectOffset;
  table_info_t    privateDictInfo;
};

கட்டமைப்பு cff1_உச்சி_dict_opset_t : உச்சி_dict_opset_t<cff1_உச்சி_dict_val_t>
{
  நிலையான வெறுமை process_op (op_code_t op, cff1_உச்சி_dict_interp_env_t& env, cff1_உச்சி_dict_மதிப்புகள்_t& dictval)
  {
    cff1_உச்சி_dict_val_t  val {};

    தேர்வு (op) {
      நிலை OpCode_version:
      நிலை OpCode_Notice:
      நிலை OpCode_Copyright:
      நிலை OpCode_FullName:
      நிலை OpCode_FontName:
      நிலை OpCode_FamilyName:
      நிலை OpCode_Weight:
      நிலை OpCode_PostScript:
      நிலை OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_மதிப்புகள்_t::name_op_to_index (op)] = env.arg_அடுக்கு.எடு_uint ();
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_isFixedPitch:
      நிலை OpCode_ItalicAngle:
      நிலை OpCode_UnderlinePosition:
      நிலை OpCode_UnderlineThickness:
      நிலை OpCode_PaintType:
      நிலை OpCode_CharstringType:
      நிலை OpCode_UniqueID:
      நிலை OpCode_StrokeWidth:
      நிலை OpCode_SyntheticBase:
      நிலை OpCode_CIDFontVersion:
      நிலை OpCode_CIDFontRevision:
      நிலை OpCode_CIDFontType:
      நிலை OpCode_UIDBase:
      நிலை OpCode_FontBBox:
      நிலை OpCode_XUID:
      நிலை OpCode_BaseFontBlend:
	env.clear_args ();
	நிறுத்து;

      நிலை OpCode_CIDCount:
	dictval.cid_மொத்தம் = env.arg_அடுக்கு.எடு_uint ();
	env.clear_args ();
	நிறுத்து;

      நிலை OpCode_ROS:
	dictval.ros_supplement = env.arg_அடுக்கு.எடு_uint ();
	dictval.nameSIDs[name_dict_மதிப்புகள்_t::ordering] = env.arg_அடுக்கு.எடு_uint ();
	dictval.nameSIDs[name_dict_மதிப்புகள்_t::registry] = env.arg_அடுக்கு.எடு_uint ();
	env.clear_args ();
	எனில் (unlikely (env.in_error ())) திரும்பு;
	ப்ரதிஜ்ஞா (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	நிறுத்து;

      நிலை OpCode_Encoding:
	dictval.EncodingOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	எனில் (unlikely (dictval.EncodingOffset == 0)) திரும்பு;
	நிறுத்து;

      நிலை OpCode_charset:
	dictval.CharsetOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	எனில் (unlikely (dictval.CharsetOffset == 0)) திரும்பு;
	நிறுத்து;

      நிலை OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;

      நிலை OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_அடுக்கு.எடு_int ();
	dictval.privateDictInfo.size = env.arg_அடுக்கு.எடு_uint ();
	env.clear_args ();
	நிறுத்து;

      இயல்பு:
	env.last_offset = env.str_ref.get_offset ();
	உச்சி_dict_opset_t<cff1_உச்சி_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
	நிறுத்து;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    dictval.add_op (op, env.str_ref, val);
  }
};

கட்டமைப்பு cff1_font_dict_மதிப்புகள்_t : dict_மதிப்புகள்_t<op_str_t>
{
  வெறுமை init ()
  {
    dict_மதிப்புகள்_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  வெறுமை fini () { dict_மதிப்புகள்_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned முழு    fontName;
};

கட்டமைப்பு cff1_font_dict_opset_t : dict_opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, இலக்கம்_interp_env_t& env, cff1_font_dict_மதிப்புகள்_t& dictval)
  {
    தேர்வு (op) {
      நிலை OpCode_FontName:
	dictval.fontName = env.arg_அடுக்கு.எடு_uint ();
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_FontMatrix:
      நிலை OpCode_PaintType:
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_அடுக்கு.எடு_uint ();
	dictval.privateDictInfo.size = env.arg_அடுக்கு.எடு_uint ();
	env.clear_args ();
	நிறுத்து;

      இயல்பு:
	dict_opset_t::process_op (op, env);
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
	நிறுத்து;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
கட்டமைப்பு cff1_private_dict_மதிப்புகள்_base_t : dict_மதிப்புகள்_t<VAL>
{
  வெறுமை init ()
  {
    dict_மதிப்புகள்_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  வெறுமை fini () { dict_மதிப்புகள்_t<VAL>::fini (); }

  முழு                 subrsOffset;
  மாறிலி CFF1Subrs    *localSubrs;
};

வகைவரை cff1_private_dict_மதிப்புகள்_base_t<op_str_t> cff1_private_dict_மதிப்புகள்_subset_t;
வகைவரை cff1_private_dict_மதிப்புகள்_base_t<இலக்கம்_dict_val_t> cff1_private_dict_மதிப்புகள்_t;

கட்டமைப்பு cff1_private_dict_opset_t : dict_opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, இலக்கம்_interp_env_t& env, cff1_private_dict_மதிப்புகள்_t& dictval)
  {
    இலக்கம்_dict_val_t val;
    val.init ();

    தேர்வு (op) {
      நிலை Op_Code_Blue_மதிப்புகள்:
      நிலை OpCode_OtherBlues:
      நிலை OpCode_FamilyBlues:
      நிலை OpCode_FamilyOtherBlues:
      நிலை OpCode_StemSnapH:
      நிலை OpCode_StemSnapV:
      நிலை OpCode_StdHW:
      நிலை OpCode_StdVW:
      நிலை OpCode_BlueScale:
      நிலை OpCode_BlueShift:
      நிலை OpCode_BlueFuzz:
      நிலை OpCode_ForceBold:
      நிலை OpCode_LanguageGroup:
      நிலை OpCode_ExpansionFactor:
      நிலை OpCode_initialRandomSeed:
      நிலை OpCode_defaultWidthX:
      நிலை OpCode_nominalWidthX:
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_Subrs:
	dictval.subrsOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;

      இயல்பு:
	dict_opset_t::process_op (op, env);
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
	நிறுத்து;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    dictval.add_op (op, env.str_ref, val);
  }
};

கட்டமைப்பு cff1_private_dict_opset_subset_t : dict_opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, இலக்கம்_interp_env_t& env, cff1_private_dict_மதிப்புகள்_subset_t& dictval)
  {
    தேர்வு (op) {
      நிலை Op_Code_Blue_மதிப்புகள்:
      நிலை OpCode_OtherBlues:
      நிலை OpCode_FamilyBlues:
      நிலை OpCode_FamilyOtherBlues:
      நிலை OpCode_StemSnapH:
      நிலை OpCode_StemSnapV:
      நிலை OpCode_StdHW:
      நிலை OpCode_StdVW:
      நிலை OpCode_BlueScale:
      நிலை OpCode_BlueShift:
      நிலை OpCode_BlueFuzz:
      நிலை OpCode_ForceBold:
      நிலை OpCode_LanguageGroup:
      நிலை OpCode_ExpansionFactor:
      நிலை OpCode_initialRandomSeed:
      நிலை OpCode_defaultWidthX:
      நிலை OpCode_nominalWidthX:
	env.clear_args ();
	நிறுத்து;

      நிலை OpCode_Subrs:
	dictval.subrsOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;

      இயல்பு:
	dict_opset_t::process_op (op, env);
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
	நிறுத்து;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    dictval.add_op (op, env.str_ref);
  }
};

வகைவரை dict_interpreter_t<cff1_உச்சி_dict_opset_t, cff1_உச்சி_dict_மதிப்புகள்_t, cff1_உச்சி_dict_interp_env_t> cff1_உச்சி_dict_interpreter_t;
வகைவரை dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_மதிப்புகள்_t> cff1_font_dict_interpreter_t;

வகைவரை CFF1Index CFF1NameIndex;
வகைவரை CFF1Index CFF1_உச்சி_Dict_Index;

கட்டமைப்பு cff1_font_dict_மதிப்புகள்_mod_t
{
  cff1_font_dict_மதிப்புகள்_mod_t() { init (); }

  வெறுமை init () { init ( &Null (cff1_font_dict_மதிப்புகள்_t), CFF_UNDEF_SID ); }

  வெறுமை init (மாறிலி cff1_font_dict_மதிப்புகள்_t *base_,
	     unsigned முழு fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_மொத்தம் () மாறிலி { திரும்பு base->get_மொத்தம் (); }

  மாறிலி op_str_t &operator [] (unsigned முழு i) மாறிலி { திரும்பு (*base)[i]; }

  மாறிலி cff1_font_dict_மதிப்புகள்_t    *base;
  table_info_t		   privateDictInfo;
  unsigned முழு		fontName;
};

கட்டமைப்பு CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  தருக்க serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { திரும்பு FDArray<HBUINT16>::serialize<cff1_font_dict_மதிப்புகள்_mod_t, cff1_font_dict_மதிப்புகள்_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

நாமாகாஶ OT {

using நாமாகாஶ CFF;

கட்டமைப்பு cff1
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  கட்டமைப்பு accelerator_templ_t
  {
    நிலையான constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      எனில் (!face) திரும்பு;

      உச்சி_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      இது->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (இது->blob);
      sc.start_processing ();

      மாறிலி OT::cff1 *cff = இது->blob->template as<OT::cff1> ();

      எனில் (cff == &Null (OT::cff1))
        செல் fail;

      nameIndex = &cff->nameIndex (cff);
      எனில் ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        செல் fail;
      hb_barrier ();

      உச்சி_Dict_Index = &StructAtOffsetOrNull<CFF1_உச்சி_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      எனில் (உச்சி_Dict_Index == &Null (CFF1_உச்சி_Dict_Index) || (உச்சி_Dict_Index->மொத்தம் == 0))
        செல் fail;
      hb_barrier ();

      { /* parse top dict */
	மாறிலி hb_ubytes_t உச்சி_Dict_Str = (*உச்சி_Dict_Index)[0];
	எனில் (unlikely (!உச்சி_Dict_Str.sanitize (&sc)))   செல் fail;
	hb_barrier ();
	cff1_உச்சி_dict_interp_env_t env (உச்சி_Dict_Str);
	cff1_உச்சி_dict_interpreter_t உச்சி_interp (env);
	எனில் (unlikely (!உச்சி_interp.interpret (உச்சி_Dict_2)))   செல் fail;
      }

      எனில் (is_predef_charset ())
	charset = &Null (Charset);
      இல்லையெனில்
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, உச்சி_Dict_2.CharsetOffset, sc, &இலக்கம்_charset_entries);
	எனில் (unlikely (charset == &Null (Charset)))   செல் fail;
      }

      fd_மொத்தம் = 1;
      எனில் (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, உச்சி_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, உச்சி_Dict_2.FDSelectOffset, sc, fdArray->மொத்தம்);
	எனில் (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  செல் fail;

	fd_மொத்தம் = fdArray->மொத்தம்;
      }
      இல்லையெனில்
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      எனில் (is_CID ())
      {
	எனில் (unlikely (charset == &Null (Charset)))   செல் fail;
      }
      இல்லையெனில்
      {
	எனில் (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, உச்சி_Dict_2.EncodingOffset, sc);
	  எனில் (unlikely (encoding == &Null (Encoding)))   செல் fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (உச்சி_Dict_Index, உச்சி_Dict_Index->get_size (), sc);
      எனில் (stringIndex == &Null (CFF1StringIndex))
        செல் fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, உச்சி_Dict_2.charStringsOffset, sc);
      எனில் (charStrings == &Null (CFF1CharStrings))
        செல் fail;

      இலக்கம்_glyphs = charStrings->மொத்தம்;
      எனில் (இலக்கம்_glyphs != sc.get_இலக்கம்_glyphs ())
        செல் fail;

      எனில் (unlikely (!privateDicts.resize (fd_மொத்தம்)))
        செல் fail;
      க்கு (unsigned முழு i = 0; i < fd_மொத்தம்; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      எனில் (is_CID ())
      {
	க்கு (unsigned முழு i = 0; i < fd_மொத்தம்; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  எனில் (unlikely (!fontDictStr.sanitize (&sc)))   செல் fail;
	  hb_barrier ();
	  cff1_font_dict_மதிப்புகள்_t *font;
	  cff1_உச்சி_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  எனில் (unlikely (fontDicts.in_error ()))   செல் fail;

	  font->init ();
	  எனில் (unlikely (!font_interp.interpret (*font)))   செல் fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  மாறிலி hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  எனில் (unlikely (font->privateDictInfo.size &&
			privDictStr == (மாறிலி unsigned எழுத்து *) &Null (UnsizedByteStr))) செல் fail;
	  இலக்கம்_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  எனில் (unlikely (!priv_interp.interpret (*priv)))   செல் fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      இல்லையெனில்  /* non-CID */
      {
	cff1_உச்சி_dict_மதிப்புகள்_t *font = &உச்சி_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	மாறிலி hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	எனில் (font->privateDictInfo.size &&
	    unlikely (privDictStr == (மாறிலி unsigned எழுத்து *) &Null (UnsizedByteStr))) செல் fail;
	இலக்கம்_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	எனில் (unlikely (!priv_interp.interpret (*priv)))   செல் fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      திரும்பு;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    வெறுமை _fini ()
    {
      sc.end_processing ();
      உச்சி_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () மாறிலி { திரும்பு blob; }

    தருக்க is_valid () மாறிலி { திரும்பு blob; }
    தருக்க   is_CID () மாறிலி { திரும்பு உச்சி_Dict_2.is_CID (); }

    தருக்க is_predef_charset () மாறிலி { திரும்பு உச்சி_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned முழு std_code_to_glyph (hb_codepoint_t code) மாறிலி
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      எனில் (unlikely (sid == CFF_UNDEF_SID))
	திரும்பு 0;

      எனில் (charset != &Null (Charset))
	திரும்பு charset->get_glyph (sid, இலக்கம்_glyphs);
      இல்லையெனில் எனில் ((உச்சி_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) திரும்பு sid;
      திரும்பு 0;
    }

    தருக்க is_predef_encoding () மாறிலி { திரும்பு உச்சி_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) மாறிலி
    {
      எனில் (encoding != &Null (Encoding))
	திரும்பு encoding->get_code (glyph);
      இல்லையெனில்
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	எனில் (sid == 0) திரும்பு 0;
	hb_codepoint_t code = 0;
	தேர்வு (உச்சி_Dict_2.EncodingOffset)
	{
	நிலை StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  நிறுத்து;
	நிலை ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  நிறுத்து;
	இயல்பு:
	  நிறுத்து;
	}
	திரும்பு code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () மாறிலி
    {
      எனில் (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (அளவு (glyph_to_sid_map_t));
	எனில் (unlikely (!mapping)) திரும்பு nullptr;
	mapping = புதிய (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, இலக்கம்_glyphs);
	திரும்பு mapping;
      }
      இல்லையெனில்
	திரும்பு nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) மாறிலி
    {
      எனில் (charset != &Null (Charset))
	திரும்பு charset->get_sid (glyph, இலக்கம்_glyphs, cache);
      இல்லையெனில்
      {
	hb_codepoint_t sid = 0;
	தேர்வு (உச்சி_Dict_2.CharsetOffset)
	{
	  நிலை ISOAdobeCharset:
	    எனில் (glyph <= 228 /*zcaron*/) sid = glyph;
	    நிறுத்து;
	  நிலை ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    நிறுத்து;
	  நிலை ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    நிறுத்து;
	  இயல்பு:
	    நிறுத்து;
	}
	திரும்பு sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) மாறிலி
    {
      எனில் (charset != &Null (Charset))
	திரும்பு charset->get_glyph (sid, இலக்கம்_glyphs);
      இல்லையெனில்
      {
	hb_codepoint_t glyph = 0;
	தேர்வு (உச்சி_Dict_2.CharsetOffset)
	{
	  நிலை ISOAdobeCharset:
	    எனில் (sid <= 228 /*zcaron*/) glyph = sid;
	    நிறுத்து;
	  நிலை ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    நிறுத்து;
	  நிலை ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    நிறுத்து;
	  இயல்பு:
	    நிறுத்து;
	}
	திரும்பு glyph;
      }
    }

    பாதுகாக்கப்பட்ட:
    hb_sanitize_context_t   sc;

    பொது:
    hb_blob_t               *blob = nullptr;
    மாறிலி Encoding	    *encoding = nullptr;
    மாறிலி Charset	    *charset = nullptr;
    மாறிலி CFF1NameIndex     *nameIndex = nullptr;
    மாறிலி CFF1_உச்சி_Dict_Index  *உச்சி_Dict_Index = nullptr;
    மாறிலி CFF1StringIndex   *stringIndex = nullptr;
    மாறிலி CFF1Subrs	    *globalSubrs = nullptr;
    மாறிலி CFF1CharStrings   *charStrings = nullptr;
    மாறிலி CFF1FDArray       *fdArray = nullptr;
    மாறிலி CFF1FDSelect      *fdSelect = nullptr;
    unsigned முழு	     fd_மொத்தம் = 0;

    cff1_உச்சி_dict_மதிப்புகள்_t   உச்சி_Dict_2;
    hb_vector_t<cff1_font_dict_மதிப்புகள்_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned முழு	     இலக்கம்_glyphs = 0;
    unsigned முழு	     இலக்கம்_charset_entries = 0;
  };

  கட்டமைப்பு accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_மதிப்புகள்_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      எனில் (!is_valid ()) திரும்பு;
      எனில் (is_CID ()) திரும்பு;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      எனில் (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    தருக்க get_glyph_name (hb_codepoint_t glyph,
			 எழுத்து *buf, unsigned முழு buf_len) மாறிலி
    {
      எனில் (unlikely (glyph >= இலக்கம்_glyphs)) திரும்பு பொய்;
      எனில் (unlikely (!is_valid ())) திரும்பு பொய்;
      எனில் (is_CID()) திரும்பு பொய்;
      எனில் (unlikely (!buf_len)) திரும்பு உண்மை;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      மாறிலி எழுத்து *str;
      size_t str_len;
      எனில் (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      இல்லையெனில்
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (மாறிலி எழுத்து *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      எனில் (!str_len) திரும்பு பொய்;
      unsigned முழு len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (மாறிலி எழுத்து*)str, len);
      buf[len] = '\0';
      திரும்பு உண்மை;
    }

    தருக்க get_glyph_from_name (மாறிலி எழுத்து *name, முழு len,
			      hb_codepoint_t *glyph) மாறிலி
    {
      எனில் (unlikely (!is_valid ())) திரும்பு பொய்;
      எனில் (is_CID()) திரும்பு பொய்;
      எனில் (len < 0) len = strlen (name);
      எனில் (unlikely (!len)) திரும்பு பொய்;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      எனில் (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, அளவு (hb_sorted_vector_t<gname_t>));
	எனில் (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  க்கு (hb_codepoint_t gid = 0; gid < இலக்கம்_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    எனில் (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    இல்லையெனில்
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((மாறிலி எழுத்து*) ustr.arrayZ, ustr.length);
	    }
	    எனில் (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	எனில் (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  எனில் (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  செல் retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      மாறிலி gname_t *gname = names ? names->bsearch (key) : nullptr;
      எனில் (!gname) திரும்பு பொய்;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      எனில் (!gid && gname->sid) திரும்பு பொய்;
      *glyph = gid;
      திரும்பு உண்மை;
    }

    HB_INTERNAL தருக்க get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) மாறிலி;
    HB_INTERNAL தருக்க get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) மாறிலி;

    தனிப்பட்ட:
    கட்டமைப்பு gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      நிலையான முழு cmp (மாறிலி வெறுமை *a_, மாறிலி வெறுமை *b_)
      {
	மாறிலி gname_t *a = (மாறிலி gname_t *)a_;
	மாறிலி gname_t *b = (மாறிலி gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	முழு ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	எனில் (ret) திரும்பு ret;
	திரும்பு a->name.length - b->name.length;
      }

      முழு cmp (மாறிலி gname_t &a) மாறிலி { திரும்பு cmp (&a, இது); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    வகைவரை accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_மதிப்புகள்_t> SUPER;
  };

  கட்டமைப்பு accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_மதிப்புகள்_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      எனில் (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL தருக்க subset (hb_subset_context_t *c) மாறிலி;
    HB_INTERNAL தருக்க serialize (hb_serialize_context_t *c,
				கட்டமைப்பு cff1_subset_plan &plan) மாறிலி;
    HB_INTERNAL தருக்க get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) மாறிலி;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    வகைவரை accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_மதிப்புகள்_subset_t> SUPER;
  };

  பாதுகாக்கப்பட்ட:
  HB_INTERNAL நிலையான hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL நிலையான hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL நிலையான hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL நிலையான hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL நிலையான hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL நிலையான hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL நிலையான hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  பொது:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

கட்டமைப்பு cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  வெறுமை depend (hb_depend_data_builder_t *builder) மாறிலி
  {
    எனில் (!is_valid ()) திரும்பு;
    க்கு (hb_codepoint_t gid = 0; gid < இலக்கம்_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      எனில் (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
