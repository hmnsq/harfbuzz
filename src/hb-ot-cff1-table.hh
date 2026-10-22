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

#અન્તર્ભાવ "hb-ot-cff-common.hh"
#અન્તર્ભાવ "hb-depend-data.hh"
#અન્તર્ભાવ "hb-subset-cff-common.hh"
#અન્તર્ભાવ "hb-draw.hh"
#અન્તર્ભાવ "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#અન્તર્ભાવ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

નામાકાશ CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

ગણના EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
ગણના CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

પ્રકારવ્યાખ્યા CFF1Index          CFF1CharStrings;
પ્રકારવ્યાખ્યા Subrs<HBUINT16>    CFF1Subrs;

સંરચના CFF1FDSelect : FDSelect {};

/* Encoding */
સંરચના Encoding0 {
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) અચલ
  {
    પ્રતિજ્ઞા (glyph > 0);
    glyph--;
    જો (glyph < nCodes ())
    {
      પાછા (hb_codepoint_t)codes[glyph];
    }
    નહીંતર
      પાછા CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { પાછા codes.len; }
  HBUINT8 nCodes () અચલ { પાછા codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

સંરચના Encoding1_Range {
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  HBUINT8   first;
  HBUINT8   n_ડાબું;

  DEFINE_SIZE_STATIC (2);
};

સંરચના Encoding1 {
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) અચલ
  {
    /* TODO: Add cache like get_sid. */
    પ્રતિજ્ઞા (glyph > 0);
    glyph--;
    માટે (unsigned પૂર્ણાંક i = 0; i < nRanges (); i++)
    {
      જો (glyph <= ranges[i].n_ડાબું)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	પાછા (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_ડાબું + 1);
    }
    પાછા CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { પાછા ranges.len; }
  HBUINT8 nRanges () અચલ { પાછા ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

સંરચના SuppEncoding {
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

સંરચના CFF1SuppEncData {
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (supps.sanitize (c));
  }

  શૂન્ય get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) અચલ
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < nSups (); i++)
      જો (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { પાછા supps.len; }
  HBUINT8 nSups () અચલ { પાછા supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

સંરચના Encoding
{
  /* serialize a fullset Encoding */
  તાર્કિક serialize (hb_serialize_context_t *c, અચલ Encoding &src)
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  તાર્કિક serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned પૂર્ણાંક enc_ગણતરી,
		  અચલ hb_vector_t<code_pair_t>& code_ranges,
		  અચલ hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (આ);
    Encoding *dest = c->extend_min (આ);
    જો (unlikely (!dest)) return_trace (ખોટું);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    પસંદગી (format) {
    સ્થિતિ 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_ગણતરી);
      જો (unlikely (!fmt0)) return_trace (ખોટું);
      fmt0->nCodes () = enc_ગણતરી;
      unsigned પૂર્ણાંક glyph = 0;
      માટે (unsigned પૂર્ણાંક i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	માટે (પૂર્ણાંક left = (પૂર્ણાંક)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	જો (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (ખોટું);
      }
    }
    તોડો;

    સ્થિતિ 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      જો (unlikely (!fmt1)) return_trace (ખોટું);
      fmt1->nRanges () = code_ranges.length;
      માટે (unsigned પૂર્ણાંક i = 0; i < code_ranges.length; i++)
      {
	જો (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (ખોટું);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_ડાબું = code_ranges[i].glyph;
      }
    }
    તોડો;

    }

    જો (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      જો (unlikely (!suppData)) return_trace (ખોટું);
      suppData->nSups () = supp_codes.length;
      માટે (unsigned પૂર્ણાંક i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (સાચું);
  }

  size_t get_size () અચલ
  {
    size_t size = min_size;
    પસંદગી (table_format ())
    {
    સ્થિતિ 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); તોડો;
    સ્થિતિ 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); તોડો;
    }
    જો (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    પાછા size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) અચલ
  {
    પસંદગી (table_format ())
    {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_code (glyph);
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_code (glyph);
    મૂળભૂત:પાછા 0;
    }
  }

  uint8_t table_format () અચલ { પાછા format & 0x7F; }
  તાર્કિક  has_supplement () અચલ { પાછા format & 0x80; }

  શૂન્ય get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) અચલ
  {
    codes.clear ();
    જો (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ)))
      return_trace (ખોટું);
    hb_barrier ();

    પસંદગી (table_format ())
    {
    સ્થિતિ 0: hb_barrier (); જો (unlikely (!u.format0.sanitize (c))) { return_trace (ખોટું); } તોડો;
    સ્થિતિ 1: hb_barrier (); જો (unlikely (!u.format1.sanitize (c))) { return_trace (ખોટું); } તોડો;
    મૂળભૂત:return_trace (ખોટું);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  સુરક્ષિત:
  અચલ CFF1SuppEncData &suppEncData () અચલ
  {
    પસંદગી (table_format ())
    {
    સ્થિતિ 0: hb_barrier (); પાછા StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    સ્થિતિ 1: hb_barrier (); પાછા StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    મૂળભૂત:પાછા Null (CFF1SuppEncData);
    }
  }

  જાહેર:
  HBUINT8	format;
  સંઘ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
સંરચના Charset0
{
  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક અંક_glyphs, unsigned *અંક_charset_entries) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (અંક_charset_entries) *અંક_charset_entries = અંક_glyphs;
    return_trace (sids.sanitize (c, અંક_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned અંક_glyphs) અચલ
  {
    જો (unlikely (glyph >= અંક_glyphs)) પાછા 0;
    જો (unlikely (glyph == 0))
      પાછા 0;
    નહીંતર
      પાછા sids[glyph - 1];
  }

  શૂન્ય collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    જો (!mapping->resize_dirty  (અંક_glyphs)) પાછા;
    માટે (hb_codepoint_t gid = 1; gid < અંક_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    જો (sid == 0)
      પાછા 0;

    માટે (unsigned પૂર્ણાંક glyph = 1; glyph < અંક_glyphs; glyph++)
    {
      જો (sids[glyph-1] == sid)
	પાછા glyph;
    }
    પાછા 0;
  }

  સ્થિર size_t get_size (unsigned પૂર્ણાંક અંક_glyphs)
  {
    પ્રતિજ્ઞા (અંક_glyphs > 0);
    પાછા UnsizedArrayOf<HBUINT16>::get_size (અંક_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
સંરચના Charset_Range {
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  HBUINT16  first;
  TYPE      n_ડાબું;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
સંરચના Charset1_2 {
  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક અંક_glyphs, unsigned *અંક_charset_entries) અચલ
  {
    TRACE_SANITIZE (આ);
    અંક_glyphs--;
    unsigned i;
    માટે (i = 0; અંક_glyphs > 0; i++)
    {
      જો (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (અંક_glyphs >= ranges[i].n_ડાબું + 1))))
	return_trace (ખોટું);
      અંક_glyphs -= (ranges[i].n_ડાબું + 1);
    }
    જો (અંક_charset_entries)
      *અંક_charset_entries = i;
    return_trace (સાચું);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned અંક_glyphs,
			  code_pair_t *cache = nullptr) અચલ
  {
    જો (unlikely (glyph >= અંક_glyphs)) પાછા 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    જો (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    નહીંતર
    {
      જો (unlikely (glyph == 0)) પાછા 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    માટે (;; i++)
    {
      unsigned ગણતરી = ranges[i].n_ડાબું;
      જો (glyph <= ગણતરી)
      {
        જો (cache)
	  *cache = {i, start_glyph};
	પાછા ranges[i].first + glyph;
      }
      ગણતરી++;
      start_glyph += ગણતરી;
      glyph -= ગણતરી;
    }

    પાછા 0;
  }

  શૂન્ય collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    જો (!mapping->resize_dirty  (અંક_glyphs)) પાછા;

    hb_codepoint_t gid = 1;
    જો (gid >= અંક_glyphs)
      પાછા;

    માટે (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned ગણતરી = ranges[i].n_ડાબું + 1;
      unsigned last = gid + ગણતરી;
      માટે (unsigned j = 0; j < ગણતરી; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      જો (gid >= અંક_glyphs)
        તોડો;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    જો (sid == 0) પાછા 0;
    hb_codepoint_t  glyph = 1;
    માટે (unsigned પૂર્ણાંક i = 0;; i++)
    {
      જો (glyph >= અંક_glyphs)
	પાછા 0;
      જો ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_ડાબું))
	પાછા glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_ડાબું + 1);
    }

    પાછા 0;
  }

  size_t get_size (unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પૂર્ણાંક glyph = (પૂર્ણાંક) અંક_glyphs;
    unsigned અંક_ranges = 0;

    પ્રતિજ્ઞા (glyph > 0);
    glyph--;
    માટે (unsigned પૂર્ણાંક i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_ડાબું + 1);
      અંક_ranges++;
    }

    પાછા get_size_for_ranges (અંક_ranges);
  }

  સ્થિર size_t get_size_for_ranges (unsigned પૂર્ણાંક અંક_ranges)
  {
    પાછા UnsizedArrayOf<Charset_Range<TYPE> >::get_size (અંક_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

પ્રકારવ્યાખ્યા Charset1_2<HBUINT8>     Charset1;
પ્રકારવ્યાખ્યા Charset1_2<HBUINT16>    Charset2;
પ્રકારવ્યાખ્યા Charset_Range<HBUINT8>  Charset1_Range;
પ્રકારવ્યાખ્યા Charset_Range<HBUINT16> Charset2_Range;

સંરચના Charset
{
  /* serialize a fullset Charset */
  તાર્કિક serialize (hb_serialize_context_t *c, અચલ Charset &src, unsigned પૂર્ણાંક અંક_glyphs)
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed ((અચલ અક્ષર *) &src, src.get_size (અંક_glyphs)));
  }

  /* serialize a subset Charset */
  તાર્કિક serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned પૂર્ણાંક અંક_glyphs,
		  અચલ hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (આ);
    Charset *dest = c->extend_min (આ);
    જો (unlikely (!dest)) return_trace (ખોટું);
    dest->format = format;
    પસંદગી (format)
    {
    સ્થિતિ 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (અંક_glyphs), ખોટું);
      જો (unlikely (!fmt0)) return_trace (ખોટું);
      unsigned પૂર્ણાંક glyph = 0;
      માટે (unsigned પૂર્ણાંક i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	માટે (પૂર્ણાંક left = (પૂર્ણાંક)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    તોડો;

    સ્થિતિ 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), ખોટું);
      જો (unlikely (!fmt1)) return_trace (ખોટું);
      hb_codepoint_t all_glyphs = 0;
      માટે (unsigned પૂર્ણાંક i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_ડાબું = _.glyph;
      }
      જો (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (ખોટું);
    }
    તોડો;

    સ્થિતિ 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), ખોટું);
      જો (unlikely (!fmt2)) return_trace (ખોટું);
      hb_codepoint_t all_glyphs = 0;
      માટે (unsigned પૂર્ણાંક i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_ડાબું = _.glyph;
      }
      જો (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (ખોટું);
    }
    તોડો;

    }
    return_trace (સાચું);
  }

  size_t get_size (unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા hb_unsigned_add_saturate (min_size, u.format0.get_size (અંક_glyphs));
    સ્થિતિ 1: hb_barrier (); પાછા hb_unsigned_add_saturate (min_size, u.format1.get_size (અંક_glyphs));
    સ્થિતિ 2: hb_barrier (); પાછા hb_unsigned_add_saturate (min_size, u.format2.get_size (અંક_glyphs));
    મૂળભૂત:પાછા 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned પૂર્ણાંક અંક_glyphs,
			  code_pair_t *cache = nullptr) અચલ
  {
    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_sid (glyph, અંક_glyphs);
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_sid (glyph, અંક_glyphs, cache);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_sid (glyph, અંક_glyphs, cache);
    મૂળભૂત:પાછા 0;
    }
  }

  શૂન્ય collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, અંક_glyphs); પાછા;
    સ્થિતિ 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, અંક_glyphs); પાછા;
    સ્થિતિ 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, અંક_glyphs); પાછા;
    મૂળભૂત:પાછા;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_glyph (sid, અંક_glyphs);
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_glyph (sid, અંક_glyphs);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_glyph (sid, અંક_glyphs);
    મૂળભૂત:પાછા 0;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned *અંક_charset_entries) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ)))
      return_trace (ખોટું);
    hb_barrier ();

    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_અંક_glyphs (), અંક_charset_entries));
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_અંક_glyphs (), અંક_charset_entries));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_અંક_glyphs (), અંક_charset_entries));
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  HBUINT8       format;
  સંઘ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

સંરચના CFF1StringIndex : CFF1Index
{
  તાર્કિક serialize (hb_serialize_context_t *c, અચલ CFF1StringIndex &strings,
		  અચલ hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely ((strings.ગણતરી == 0) || (sidmap.length == 0)))
    {
      જો (unlikely (!c->extend_min (આ->ગણતરી)))
	return_trace (ખોટું);
      ગણતરી = 0;
      return_trace (સાચું);
    }

    જો (unlikely (sidmap.in_error ())) return_trace (ખોટું);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    જો (unlikely (bytesArray.in_error ())) return_trace (ખોટું);

    તાર્કિક result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

સંરચના cff1_ટોચ_dict_interp_env_t : અંક_interp_env_t
{
  cff1_ટોચ_dict_interp_env_t ()
    : અંક_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_ટોચ_dict_interp_env_t (અચલ hb_ubytes_t &bytes)
    : અંક_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned પૂર્ણાંક prev_offset;
  unsigned પૂર્ણાંક last_offset;
};

સંરચના name_dict_કિંમતો_t
{
  ગણના name_dict_val_index_t
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

      Val_ગણતરી
  };

  શૂન્ય init ()
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < Val_ગણતરી; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned પૂર્ણાંક& operator[] (unsigned પૂર્ણાંક i)
  { પ્રતિજ્ઞા (i < Val_ગણતરી); પાછા values[i]; }

  unsigned પૂર્ણાંક operator[] (unsigned પૂર્ણાંક i) અચલ
  { પ્રતિજ્ઞા (i < Val_ગણતરી); પાછા values[i]; }

  સ્થિર ગણના name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    પસંદગી (op) {
      મૂળભૂત: // can't happen - just make some compiler happy
      સ્થિતિ OpCode_version:
	પાછા version;
      સ્થિતિ OpCode_Notice:
	પાછા notice;
      સ્થિતિ OpCode_Copyright:
	પાછા copyright;
      સ્થિતિ OpCode_FullName:
	પાછા fullName;
      સ્થિતિ OpCode_FamilyName:
	પાછા familyName;
      સ્થિતિ OpCode_Weight:
	પાછા weight;
      સ્થિતિ OpCode_PostScript:
	પાછા postscript;
      સ્થિતિ OpCode_FontName:
	પાછા fontName;
      સ્થિતિ OpCode_BaseFontName:
	પાછા baseFontName;
    }
  }

  unsigned પૂર્ણાંક  values[Val_ગણતરી];
};

સંરચના cff1_ટોચ_dict_val_t : op_str_t
{
  unsigned પૂર્ણાંક  last_arg_offset;
};

સંરચના cff1_ટોચ_dict_કિંમતો_t : ટોચ_dict_કિંમતો_t<cff1_ટોચ_dict_val_t>
{
  શૂન્ય init ()
  {
    ટોચ_dict_કિંમતો_t<cff1_ટોચ_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_ગણતરી = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  શૂન્ય fini () { ટોચ_dict_કિંમતો_t<cff1_ટોચ_dict_val_t>::fini (); }

  તાર્કિક is_CID () અચલ
  { પાછા nameSIDs[name_dict_કિંમતો_t::registry] != CFF_UNDEF_SID; }

  name_dict_કિંમતો_t  nameSIDs;
  unsigned પૂર્ણાંક    ros_supplement_offset;
  unsigned પૂર્ણાંક    ros_supplement;
  unsigned પૂર્ણાંક    cid_ગણતરી;

  પૂર્ણાંક             EncodingOffset;
  પૂર્ણાંક             CharsetOffset;
  પૂર્ણાંક             FDSelectOffset;
  table_info_t    privateDictInfo;
};

સંરચના cff1_ટોચ_dict_opset_t : ટોચ_dict_opset_t<cff1_ટોચ_dict_val_t>
{
  સ્થિર શૂન્ય process_op (op_code_t op, cff1_ટોચ_dict_interp_env_t& env, cff1_ટોચ_dict_કિંમતો_t& dictval)
  {
    cff1_ટોચ_dict_val_t  val {};

    પસંદગી (op) {
      સ્થિતિ OpCode_version:
      સ્થિતિ OpCode_Notice:
      સ્થિતિ OpCode_Copyright:
      સ્થિતિ OpCode_FullName:
      સ્થિતિ OpCode_FontName:
      સ્થિતિ OpCode_FamilyName:
      સ્થિતિ OpCode_Weight:
      સ્થિતિ OpCode_PostScript:
      સ્થિતિ OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_કિંમતો_t::name_op_to_index (op)] = env.arg_ઢગલો.કાઢો_uint ();
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_isFixedPitch:
      સ્થિતિ OpCode_ItalicAngle:
      સ્થિતિ OpCode_UnderlinePosition:
      સ્થિતિ OpCode_UnderlineThickness:
      સ્થિતિ OpCode_PaintType:
      સ્થિતિ OpCode_CharstringType:
      સ્થિતિ OpCode_UniqueID:
      સ્થિતિ OpCode_StrokeWidth:
      સ્થિતિ OpCode_SyntheticBase:
      સ્થિતિ OpCode_CIDFontVersion:
      સ્થિતિ OpCode_CIDFontRevision:
      સ્થિતિ OpCode_CIDFontType:
      સ્થિતિ OpCode_UIDBase:
      સ્થિતિ OpCode_FontBBox:
      સ્થિતિ OpCode_XUID:
      સ્થિતિ OpCode_BaseFontBlend:
	env.clear_args ();
	તોડો;

      સ્થિતિ OpCode_CIDCount:
	dictval.cid_ગણતરી = env.arg_ઢગલો.કાઢો_uint ();
	env.clear_args ();
	તોડો;

      સ્થિતિ OpCode_ROS:
	dictval.ros_supplement = env.arg_ઢગલો.કાઢો_uint ();
	dictval.nameSIDs[name_dict_કિંમતો_t::ordering] = env.arg_ઢગલો.કાઢો_uint ();
	dictval.nameSIDs[name_dict_કિંમતો_t::registry] = env.arg_ઢગલો.કાઢો_uint ();
	env.clear_args ();
	જો (unlikely (env.in_error ())) પાછા;
	પ્રતિજ્ઞા (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	તોડો;

      સ્થિતિ OpCode_Encoding:
	dictval.EncodingOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	જો (unlikely (dictval.EncodingOffset == 0)) પાછા;
	તોડો;

      સ્થિતિ OpCode_charset:
	dictval.CharsetOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	જો (unlikely (dictval.CharsetOffset == 0)) પાછા;
	તોડો;

      સ્થિતિ OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;

      સ્થિતિ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ઢગલો.કાઢો_int ();
	dictval.privateDictInfo.size = env.arg_ઢગલો.કાઢો_uint ();
	env.clear_args ();
	તોડો;

      મૂળભૂત:
	env.last_offset = env.str_ref.get_offset ();
	ટોચ_dict_opset_t<cff1_ટોચ_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
	તોડો;
    }

    જો (unlikely (env.in_error ())) પાછા;

    dictval.add_op (op, env.str_ref, val);
  }
};

સંરચના cff1_font_dict_કિંમતો_t : dict_કિંમતો_t<op_str_t>
{
  શૂન્ય init ()
  {
    dict_કિંમતો_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  શૂન્ય fini () { dict_કિંમતો_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned પૂર્ણાંક    fontName;
};

સંરચના cff1_font_dict_opset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, અંક_interp_env_t& env, cff1_font_dict_કિંમતો_t& dictval)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_FontName:
	dictval.fontName = env.arg_ઢગલો.કાઢો_uint ();
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_FontMatrix:
      સ્થિતિ OpCode_PaintType:
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ઢગલો.કાઢો_uint ();
	dictval.privateDictInfo.size = env.arg_ઢગલો.કાઢો_uint ();
	env.clear_args ();
	તોડો;

      મૂળભૂત:
	dict_opset_t::process_op (op, env);
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
	તોડો;
    }

    જો (unlikely (env.in_error ())) પાછા;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
સંરચના cff1_private_dict_કિંમતો_base_t : dict_કિંમતો_t<VAL>
{
  શૂન્ય init ()
  {
    dict_કિંમતો_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  શૂન્ય fini () { dict_કિંમતો_t<VAL>::fini (); }

  પૂર્ણાંક                 subrsOffset;
  અચલ CFF1Subrs    *localSubrs;
};

પ્રકારવ્યાખ્યા cff1_private_dict_કિંમતો_base_t<op_str_t> cff1_private_dict_કિંમતો_subset_t;
પ્રકારવ્યાખ્યા cff1_private_dict_કિંમતો_base_t<અંક_dict_val_t> cff1_private_dict_કિંમતો_t;

સંરચના cff1_private_dict_opset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, અંક_interp_env_t& env, cff1_private_dict_કિંમતો_t& dictval)
  {
    અંક_dict_val_t val;
    val.init ();

    પસંદગી (op) {
      સ્થિતિ Op_Code_Blue_કિંમતો:
      સ્થિતિ OpCode_OtherBlues:
      સ્થિતિ OpCode_FamilyBlues:
      સ્થિતિ OpCode_FamilyOtherBlues:
      સ્થિતિ OpCode_StemSnapH:
      સ્થિતિ OpCode_StemSnapV:
      સ્થિતિ OpCode_StdHW:
      સ્થિતિ OpCode_StdVW:
      સ્થિતિ OpCode_BlueScale:
      સ્થિતિ OpCode_BlueShift:
      સ્થિતિ OpCode_BlueFuzz:
      સ્થિતિ OpCode_ForceBold:
      સ્થિતિ OpCode_LanguageGroup:
      સ્થિતિ OpCode_ExpansionFactor:
      સ્થિતિ OpCode_initialRandomSeed:
      સ્થિતિ OpCode_defaultWidthX:
      સ્થિતિ OpCode_nominalWidthX:
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_Subrs:
	dictval.subrsOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;

      મૂળભૂત:
	dict_opset_t::process_op (op, env);
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
	તોડો;
    }

    જો (unlikely (env.in_error ())) પાછા;

    dictval.add_op (op, env.str_ref, val);
  }
};

સંરચના cff1_private_dict_opset_subset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, અંક_interp_env_t& env, cff1_private_dict_કિંમતો_subset_t& dictval)
  {
    પસંદગી (op) {
      સ્થિતિ Op_Code_Blue_કિંમતો:
      સ્થિતિ OpCode_OtherBlues:
      સ્થિતિ OpCode_FamilyBlues:
      સ્થિતિ OpCode_FamilyOtherBlues:
      સ્થિતિ OpCode_StemSnapH:
      સ્થિતિ OpCode_StemSnapV:
      સ્થિતિ OpCode_StdHW:
      સ્થિતિ OpCode_StdVW:
      સ્થિતિ OpCode_BlueScale:
      સ્થિતિ OpCode_BlueShift:
      સ્થિતિ OpCode_BlueFuzz:
      સ્થિતિ OpCode_ForceBold:
      સ્થિતિ OpCode_LanguageGroup:
      સ્થિતિ OpCode_ExpansionFactor:
      સ્થિતિ OpCode_initialRandomSeed:
      સ્થિતિ OpCode_defaultWidthX:
      સ્થિતિ OpCode_nominalWidthX:
	env.clear_args ();
	તોડો;

      સ્થિતિ OpCode_Subrs:
	dictval.subrsOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;

      મૂળભૂત:
	dict_opset_t::process_op (op, env);
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
	તોડો;
    }

    જો (unlikely (env.in_error ())) પાછા;

    dictval.add_op (op, env.str_ref);
  }
};

પ્રકારવ્યાખ્યા dict_interpreter_t<cff1_ટોચ_dict_opset_t, cff1_ટોચ_dict_કિંમતો_t, cff1_ટોચ_dict_interp_env_t> cff1_ટોચ_dict_interpreter_t;
પ્રકારવ્યાખ્યા dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_કિંમતો_t> cff1_font_dict_interpreter_t;

પ્રકારવ્યાખ્યા CFF1Index CFF1NameIndex;
પ્રકારવ્યાખ્યા CFF1Index CFF1_ટોચ_Dict_Index;

સંરચના cff1_font_dict_કિંમતો_mod_t
{
  cff1_font_dict_કિંમતો_mod_t() { init (); }

  શૂન્ય init () { init ( &Null (cff1_font_dict_કિંમતો_t), CFF_UNDEF_SID ); }

  શૂન્ય init (અચલ cff1_font_dict_કિંમતો_t *base_,
	     unsigned પૂર્ણાંક fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_ગણતરી () અચલ { પાછા base->get_ગણતરી (); }

  અચલ op_str_t &operator [] (unsigned પૂર્ણાંક i) અચલ { પાછા (*base)[i]; }

  અચલ cff1_font_dict_કિંમતો_t    *base;
  table_info_t		   privateDictInfo;
  unsigned પૂર્ણાંક		fontName;
};

સંરચના CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  તાર્કિક serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { પાછા FDArray<HBUINT16>::serialize<cff1_font_dict_કિંમતો_mod_t, cff1_font_dict_કિંમતો_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

નામાકાશ OT {

using નામાકાશ CFF;

સંરચના cff1
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  સંરચના accelerator_templ_t
  {
    સ્થિર constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      જો (!face) પાછા;

      ટોચ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      આ->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (આ->blob);
      sc.start_processing ();

      અચલ OT::cff1 *cff = આ->blob->template as<OT::cff1> ();

      જો (cff == &Null (OT::cff1))
        જાઓ fail;

      nameIndex = &cff->nameIndex (cff);
      જો ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        જાઓ fail;
      hb_barrier ();

      ટોચ_Dict_Index = &StructAtOffsetOrNull<CFF1_ટોચ_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      જો (ટોચ_Dict_Index == &Null (CFF1_ટોચ_Dict_Index) || (ટોચ_Dict_Index->ગણતરી == 0))
        જાઓ fail;
      hb_barrier ();

      { /* parse top dict */
	અચલ hb_ubytes_t ટોચ_Dict_Str = (*ટોચ_Dict_Index)[0];
	જો (unlikely (!ટોચ_Dict_Str.sanitize (&sc)))   જાઓ fail;
	hb_barrier ();
	cff1_ટોચ_dict_interp_env_t env (ટોચ_Dict_Str);
	cff1_ટોચ_dict_interpreter_t ટોચ_interp (env);
	જો (unlikely (!ટોચ_interp.interpret (ટોચ_Dict_2)))   જાઓ fail;
      }

      જો (is_predef_charset ())
	charset = &Null (Charset);
      નહીંતર
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, ટોચ_Dict_2.CharsetOffset, sc, &અંક_charset_entries);
	જો (unlikely (charset == &Null (Charset)))   જાઓ fail;
      }

      fd_ગણતરી = 1;
      જો (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, ટોચ_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, ટોચ_Dict_2.FDSelectOffset, sc, fdArray->ગણતરી);
	જો (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  જાઓ fail;

	fd_ગણતરી = fdArray->ગણતરી;
      }
      નહીંતર
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      જો (is_CID ())
      {
	જો (unlikely (charset == &Null (Charset)))   જાઓ fail;
      }
      નહીંતર
      {
	જો (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, ટોચ_Dict_2.EncodingOffset, sc);
	  જો (unlikely (encoding == &Null (Encoding)))   જાઓ fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (ટોચ_Dict_Index, ટોચ_Dict_Index->get_size (), sc);
      જો (stringIndex == &Null (CFF1StringIndex))
        જાઓ fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, ટોચ_Dict_2.charStringsOffset, sc);
      જો (charStrings == &Null (CFF1CharStrings))
        જાઓ fail;

      અંક_glyphs = charStrings->ગણતરી;
      જો (અંક_glyphs != sc.get_અંક_glyphs ())
        જાઓ fail;

      જો (unlikely (!privateDicts.resize (fd_ગણતરી)))
        જાઓ fail;
      માટે (unsigned પૂર્ણાંક i = 0; i < fd_ગણતરી; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      જો (is_CID ())
      {
	માટે (unsigned પૂર્ણાંક i = 0; i < fd_ગણતરી; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  જો (unlikely (!fontDictStr.sanitize (&sc)))   જાઓ fail;
	  hb_barrier ();
	  cff1_font_dict_કિંમતો_t *font;
	  cff1_ટોચ_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  જો (unlikely (fontDicts.in_error ()))   જાઓ fail;

	  font->init ();
	  જો (unlikely (!font_interp.interpret (*font)))   જાઓ fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  અચલ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  જો (unlikely (font->privateDictInfo.size &&
			privDictStr == (અચલ unsigned અક્ષર *) &Null (UnsizedByteStr))) જાઓ fail;
	  અંક_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  જો (unlikely (!priv_interp.interpret (*priv)))   જાઓ fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      નહીંતર  /* non-CID */
      {
	cff1_ટોચ_dict_કિંમતો_t *font = &ટોચ_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	અચલ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	જો (font->privateDictInfo.size &&
	    unlikely (privDictStr == (અચલ unsigned અક્ષર *) &Null (UnsizedByteStr))) જાઓ fail;
	અંક_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	જો (unlikely (!priv_interp.interpret (*priv)))   જાઓ fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      પાછા;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    શૂન્ય _fini ()
    {
      sc.end_processing ();
      ટોચ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () અચલ { પાછા blob; }

    તાર્કિક is_valid () અચલ { પાછા blob; }
    તાર્કિક   is_CID () અચલ { પાછા ટોચ_Dict_2.is_CID (); }

    તાર્કિક is_predef_charset () અચલ { પાછા ટોચ_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned પૂર્ણાંક std_code_to_glyph (hb_codepoint_t code) અચલ
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      જો (unlikely (sid == CFF_UNDEF_SID))
	પાછા 0;

      જો (charset != &Null (Charset))
	પાછા charset->get_glyph (sid, અંક_glyphs);
      નહીંતર જો ((ટોચ_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) પાછા sid;
      પાછા 0;
    }

    તાર્કિક is_predef_encoding () અચલ { પાછા ટોચ_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) અચલ
    {
      જો (encoding != &Null (Encoding))
	પાછા encoding->get_code (glyph);
      નહીંતર
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	જો (sid == 0) પાછા 0;
	hb_codepoint_t code = 0;
	પસંદગી (ટોચ_Dict_2.EncodingOffset)
	{
	સ્થિતિ StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  તોડો;
	સ્થિતિ ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  તોડો;
	મૂળભૂત:
	  તોડો;
	}
	પાછા code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () અચલ
    {
      જો (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (કદ (glyph_to_sid_map_t));
	જો (unlikely (!mapping)) પાછા nullptr;
	mapping = નવું (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, અંક_glyphs);
	પાછા mapping;
      }
      નહીંતર
	પાછા nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) અચલ
    {
      જો (charset != &Null (Charset))
	પાછા charset->get_sid (glyph, અંક_glyphs, cache);
      નહીંતર
      {
	hb_codepoint_t sid = 0;
	પસંદગી (ટોચ_Dict_2.CharsetOffset)
	{
	  સ્થિતિ ISOAdobeCharset:
	    જો (glyph <= 228 /*zcaron*/) sid = glyph;
	    તોડો;
	  સ્થિતિ ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    તોડો;
	  સ્થિતિ ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    તોડો;
	  મૂળભૂત:
	    તોડો;
	}
	પાછા sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) અચલ
    {
      જો (charset != &Null (Charset))
	પાછા charset->get_glyph (sid, અંક_glyphs);
      નહીંતર
      {
	hb_codepoint_t glyph = 0;
	પસંદગી (ટોચ_Dict_2.CharsetOffset)
	{
	  સ્થિતિ ISOAdobeCharset:
	    જો (sid <= 228 /*zcaron*/) glyph = sid;
	    તોડો;
	  સ્થિતિ ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    તોડો;
	  સ્થિતિ ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    તોડો;
	  મૂળભૂત:
	    તોડો;
	}
	પાછા glyph;
      }
    }

    સુરક્ષિત:
    hb_sanitize_context_t   sc;

    જાહેર:
    hb_blob_t               *blob = nullptr;
    અચલ Encoding	    *encoding = nullptr;
    અચલ Charset	    *charset = nullptr;
    અચલ CFF1NameIndex     *nameIndex = nullptr;
    અચલ CFF1_ટોચ_Dict_Index  *ટોચ_Dict_Index = nullptr;
    અચલ CFF1StringIndex   *stringIndex = nullptr;
    અચલ CFF1Subrs	    *globalSubrs = nullptr;
    અચલ CFF1CharStrings   *charStrings = nullptr;
    અચલ CFF1FDArray       *fdArray = nullptr;
    અચલ CFF1FDSelect      *fdSelect = nullptr;
    unsigned પૂર્ણાંક	     fd_ગણતરી = 0;

    cff1_ટોચ_dict_કિંમતો_t   ટોચ_Dict_2;
    hb_vector_t<cff1_font_dict_કિંમતો_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned પૂર્ણાંક	     અંક_glyphs = 0;
    unsigned પૂર્ણાંક	     અંક_charset_entries = 0;
  };

  સંરચના accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_કિંમતો_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      જો (!is_valid ()) પાછા;
      જો (is_CID ()) પાછા;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      જો (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    તાર્કિક get_glyph_name (hb_codepoint_t glyph,
			 અક્ષર *buf, unsigned પૂર્ણાંક buf_len) અચલ
    {
      જો (unlikely (glyph >= અંક_glyphs)) પાછા ખોટું;
      જો (unlikely (!is_valid ())) પાછા ખોટું;
      જો (is_CID()) પાછા ખોટું;
      જો (unlikely (!buf_len)) પાછા સાચું;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      અચલ અક્ષર *str;
      size_t str_len;
      જો (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      નહીંતર
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (અચલ અક્ષર *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      જો (!str_len) પાછા ખોટું;
      unsigned પૂર્ણાંક len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (અચલ અક્ષર*)str, len);
      buf[len] = '\0';
      પાછા સાચું;
    }

    તાર્કિક get_glyph_from_name (અચલ અક્ષર *name, પૂર્ણાંક len,
			      hb_codepoint_t *glyph) અચલ
    {
      જો (unlikely (!is_valid ())) પાછા ખોટું;
      જો (is_CID()) પાછા ખોટું;
      જો (len < 0) len = strlen (name);
      જો (unlikely (!len)) પાછા ખોટું;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      જો (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, કદ (hb_sorted_vector_t<gname_t>));
	જો (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  માટે (hb_codepoint_t gid = 0; gid < અંક_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    જો (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    નહીંતર
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((અચલ અક્ષર*) ustr.arrayZ, ustr.length);
	    }
	    જો (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	જો (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  જો (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  જાઓ retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      અચલ gname_t *gname = names ? names->bsearch (key) : nullptr;
      જો (!gname) પાછા ખોટું;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      જો (!gid && gname->sid) પાછા ખોટું;
      *glyph = gid;
      પાછા સાચું;
    }

    HB_INTERNAL તાર્કિક get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) અચલ;
    HB_INTERNAL તાર્કિક get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) અચલ;

    ખાનગી:
    સંરચના gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *a_, અચલ શૂન્ય *b_)
      {
	અચલ gname_t *a = (અચલ gname_t *)a_;
	અચલ gname_t *b = (અચલ gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	પૂર્ણાંક ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	જો (ret) પાછા ret;
	પાછા a->name.length - b->name.length;
      }

      પૂર્ણાંક cmp (અચલ gname_t &a) અચલ { પાછા cmp (&a, આ); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    પ્રકારવ્યાખ્યા accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_કિંમતો_t> SUPER;
  };

  સંરચના accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_કિંમતો_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      જો (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL તાર્કિક subset (hb_subset_context_t *c) અચલ;
    HB_INTERNAL તાર્કિક serialize (hb_serialize_context_t *c,
				સંરચના cff1_subset_plan &plan) અચલ;
    HB_INTERNAL તાર્કિક get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) અચલ;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    પ્રકારવ્યાખ્યા accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_કિંમતો_subset_t> SUPER;
  };

  સુરક્ષિત:
  HB_INTERNAL સ્થિર hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL સ્થિર hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL સ્થિર hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL સ્થિર hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL સ્થિર hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL સ્થિર hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL સ્થિર hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  જાહેર:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

સંરચના cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  શૂન્ય depend (hb_depend_data_builder_t *builder) અચલ
  {
    જો (!is_valid ()) પાછા;
    માટે (hb_codepoint_t gid = 0; gid < અંક_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      જો (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
