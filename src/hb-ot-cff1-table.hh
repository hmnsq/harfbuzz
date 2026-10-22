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

#അന്തര്ഭാവ "hb-ot-cff-common.hh"
#അന്തര്ഭാവ "hb-depend-data.hh"
#അന്തര്ഭാവ "hb-subset-cff-common.hh"
#അന്തര്ഭാവ "hb-draw.hh"
#അന്തര്ഭാവ "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#അന്തര്ഭാവ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

നാമാകാശ CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

എണ്ണൽ EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
എണ്ണൽ CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

തരനിർവചനം CFF1Index          CFF1CharStrings;
തരനിർവചനം Subrs<HBUINT16>    CFF1Subrs;

ഘടന CFF1FDSelect : FDSelect {};

/* Encoding */
ഘടന Encoding0 {
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) സ്ഥിരം
  {
    പ്രതിജ്ഞാ (glyph > 0);
    glyph--;
    എങ്കിൽ (glyph < nCodes ())
    {
      തിരികെ (hb_codepoint_t)codes[glyph];
    }
    അല്ലെങ്കിൽ
      തിരികെ CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { തിരികെ codes.len; }
  HBUINT8 nCodes () സ്ഥിരം { തിരികെ codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

ഘടന Encoding1_Range {
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  HBUINT8   first;
  HBUINT8   n_ഇടത്;

  DEFINE_SIZE_STATIC (2);
};

ഘടന Encoding1 {
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) സ്ഥിരം
  {
    /* TODO: Add cache like get_sid. */
    പ്രതിജ്ഞാ (glyph > 0);
    glyph--;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < nRanges (); i++)
    {
      എങ്കിൽ (glyph <= ranges[i].n_ഇടത്)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	തിരികെ (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_ഇടത് + 1);
    }
    തിരികെ CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { തിരികെ ranges.len; }
  HBUINT8 nRanges () സ്ഥിരം { തിരികെ ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

ഘടന SuppEncoding {
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

ഘടന CFF1SuppEncData {
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (supps.sanitize (c));
  }

  ശൂന്യം get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) സ്ഥിരം
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < nSups (); i++)
      എങ്കിൽ (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { തിരികെ supps.len; }
  HBUINT8 nSups () സ്ഥിരം { തിരികെ supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

ഘടന Encoding
{
  /* serialize a fullset Encoding */
  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം Encoding &src)
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  തർക്കം serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned പൂർണ്ണസംഖ്യ enc_എണ്ണം,
		  സ്ഥിരം hb_vector_t<code_pair_t>& code_ranges,
		  സ്ഥിരം hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (ഇത്);
    Encoding *dest = c->extend_min (ഇത്);
    എങ്കിൽ (unlikely (!dest)) return_trace (അസത്യം);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    തിരഞ്ഞെടുപ്പ് (format) {
    അവസ്ഥ 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_എണ്ണം);
      എങ്കിൽ (unlikely (!fmt0)) return_trace (അസത്യം);
      fmt0->nCodes () = enc_എണ്ണം;
      unsigned പൂർണ്ണസംഖ്യ glyph = 0;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	വേണ്ടി (പൂർണ്ണസംഖ്യ left = (പൂർണ്ണസംഖ്യ)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	എങ്കിൽ (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (അസത്യം);
      }
    }
    നിർത്തുക;

    അവസ്ഥ 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      എങ്കിൽ (unlikely (!fmt1)) return_trace (അസത്യം);
      fmt1->nRanges () = code_ranges.length;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < code_ranges.length; i++)
      {
	എങ്കിൽ (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (അസത്യം);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_ഇടത് = code_ranges[i].glyph;
      }
    }
    നിർത്തുക;

    }

    എങ്കിൽ (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      എങ്കിൽ (unlikely (!suppData)) return_trace (അസത്യം);
      suppData->nSups () = supp_codes.length;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (സത്യം);
  }

  size_t get_size () സ്ഥിരം
  {
    size_t size = min_size;
    തിരഞ്ഞെടുപ്പ് (table_format ())
    {
    അവസ്ഥ 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); നിർത്തുക;
    അവസ്ഥ 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); നിർത്തുക;
    }
    എങ്കിൽ (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    തിരികെ size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (table_format ())
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_code (glyph);
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_code (glyph);
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  uint8_t table_format () സ്ഥിരം { തിരികെ format & 0x7F; }
  തർക്കം  has_supplement () സ്ഥിരം { തിരികെ format & 0x80; }

  ശൂന്യം get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) സ്ഥിരം
  {
    codes.clear ();
    എങ്കിൽ (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്)))
      return_trace (അസത്യം);
    hb_barrier ();

    തിരഞ്ഞെടുപ്പ് (table_format ())
    {
    അവസ്ഥ 0: hb_barrier (); എങ്കിൽ (unlikely (!u.format0.sanitize (c))) { return_trace (അസത്യം); } നിർത്തുക;
    അവസ്ഥ 1: hb_barrier (); എങ്കിൽ (unlikely (!u.format1.sanitize (c))) { return_trace (അസത്യം); } നിർത്തുക;
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  സംരക്ഷിത:
  സ്ഥിരം CFF1SuppEncData &suppEncData () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (table_format ())
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    അവസ്ഥ 1: hb_barrier (); തിരികെ StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    സ്ഥിരസ്ഥിതി:തിരികെ Null (CFF1SuppEncData);
    }
  }

  പൊതു:
  HBUINT8	format;
  സംഘം {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
ഘടന Charset0
{
  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs, unsigned *അക്കം_charset_entries) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (അക്കം_charset_entries) *അക്കം_charset_entries = അക്കം_glyphs;
    return_trace (sids.sanitize (c, അക്കം_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (glyph >= അക്കം_glyphs)) തിരികെ 0;
    എങ്കിൽ (unlikely (glyph == 0))
      തിരികെ 0;
    അല്ലെങ്കിൽ
      തിരികെ sids[glyph - 1];
  }

  ശൂന്യം collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (!mapping->resize_dirty  (അക്കം_glyphs)) തിരികെ;
    വേണ്ടി (hb_codepoint_t gid = 1; gid < അക്കം_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (sid == 0)
      തിരികെ 0;

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ glyph = 1; glyph < അക്കം_glyphs; glyph++)
    {
      എങ്കിൽ (sids[glyph-1] == sid)
	തിരികെ glyph;
    }
    തിരികെ 0;
  }

  സ്ഥിതം size_t get_size (unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs)
  {
    പ്രതിജ്ഞാ (അക്കം_glyphs > 0);
    തിരികെ UnsizedArrayOf<HBUINT16>::get_size (അക്കം_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
ഘടന Charset_Range {
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  HBUINT16  first;
  TYPE      n_ഇടത്;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
ഘടന Charset1_2 {
  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs, unsigned *അക്കം_charset_entries) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    അക്കം_glyphs--;
    unsigned i;
    വേണ്ടി (i = 0; അക്കം_glyphs > 0; i++)
    {
      എങ്കിൽ (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (അക്കം_glyphs >= ranges[i].n_ഇടത് + 1))))
	return_trace (അസത്യം);
      അക്കം_glyphs -= (ranges[i].n_ഇടത് + 1);
    }
    എങ്കിൽ (അക്കം_charset_entries)
      *അക്കം_charset_entries = i;
    return_trace (സത്യം);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned അക്കം_glyphs,
			  code_pair_t *cache = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (glyph >= അക്കം_glyphs)) തിരികെ 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    എങ്കിൽ (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (unlikely (glyph == 0)) തിരികെ 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    വേണ്ടി (;; i++)
    {
      unsigned എണ്ണം = ranges[i].n_ഇടത്;
      എങ്കിൽ (glyph <= എണ്ണം)
      {
        എങ്കിൽ (cache)
	  *cache = {i, start_glyph};
	തിരികെ ranges[i].first + glyph;
      }
      എണ്ണം++;
      start_glyph += എണ്ണം;
      glyph -= എണ്ണം;
    }

    തിരികെ 0;
  }

  ശൂന്യം collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (!mapping->resize_dirty  (അക്കം_glyphs)) തിരികെ;

    hb_codepoint_t gid = 1;
    എങ്കിൽ (gid >= അക്കം_glyphs)
      തിരികെ;

    വേണ്ടി (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned എണ്ണം = ranges[i].n_ഇടത് + 1;
      unsigned last = gid + എണ്ണം;
      വേണ്ടി (unsigned j = 0; j < എണ്ണം; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      എങ്കിൽ (gid >= അക്കം_glyphs)
        നിർത്തുക;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (sid == 0) തിരികെ 0;
    hb_codepoint_t  glyph = 1;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0;; i++)
    {
      എങ്കിൽ (glyph >= അക്കം_glyphs)
	തിരികെ 0;
      എങ്കിൽ ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_ഇടത്))
	തിരികെ glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_ഇടത് + 1);
    }

    തിരികെ 0;
  }

  size_t get_size (unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    പൂർണ്ണസംഖ്യ glyph = (പൂർണ്ണസംഖ്യ) അക്കം_glyphs;
    unsigned അക്കം_ranges = 0;

    പ്രതിജ്ഞാ (glyph > 0);
    glyph--;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_ഇടത് + 1);
      അക്കം_ranges++;
    }

    തിരികെ get_size_for_ranges (അക്കം_ranges);
  }

  സ്ഥിതം size_t get_size_for_ranges (unsigned പൂർണ്ണസംഖ്യ അക്കം_ranges)
  {
    തിരികെ UnsizedArrayOf<Charset_Range<TYPE> >::get_size (അക്കം_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

തരനിർവചനം Charset1_2<HBUINT8>     Charset1;
തരനിർവചനം Charset1_2<HBUINT16>    Charset2;
തരനിർവചനം Charset_Range<HBUINT8>  Charset1_Range;
തരനിർവചനം Charset_Range<HBUINT16> Charset2_Range;

ഘടന Charset
{
  /* serialize a fullset Charset */
  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം Charset &src, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs)
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed ((സ്ഥിരം അക്ഷരം *) &src, src.get_size (അക്കം_glyphs)));
  }

  /* serialize a subset Charset */
  തർക്കം serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
		  സ്ഥിരം hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (ഇത്);
    Charset *dest = c->extend_min (ഇത്);
    എങ്കിൽ (unlikely (!dest)) return_trace (അസത്യം);
    dest->format = format;
    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (അക്കം_glyphs), അസത്യം);
      എങ്കിൽ (unlikely (!fmt0)) return_trace (അസത്യം);
      unsigned പൂർണ്ണസംഖ്യ glyph = 0;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	വേണ്ടി (പൂർണ്ണസംഖ്യ left = (പൂർണ്ണസംഖ്യ)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    നിർത്തുക;

    അവസ്ഥ 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), അസത്യം);
      എങ്കിൽ (unlikely (!fmt1)) return_trace (അസത്യം);
      hb_codepoint_t all_glyphs = 0;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_ഇടത് = _.glyph;
      }
      എങ്കിൽ (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (അസത്യം);
    }
    നിർത്തുക;

    അവസ്ഥ 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), അസത്യം);
      എങ്കിൽ (unlikely (!fmt2)) return_trace (അസത്യം);
      hb_codepoint_t all_glyphs = 0;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_ഇടത് = _.glyph;
      }
      എങ്കിൽ (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (അസത്യം);
    }
    നിർത്തുക;

    }
    return_trace (സത്യം);
  }

  size_t get_size (unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ hb_unsigned_add_saturate (min_size, u.format0.get_size (അക്കം_glyphs));
    അവസ്ഥ 1: hb_barrier (); തിരികെ hb_unsigned_add_saturate (min_size, u.format1.get_size (അക്കം_glyphs));
    അവസ്ഥ 2: hb_barrier (); തിരികെ hb_unsigned_add_saturate (min_size, u.format2.get_size (അക്കം_glyphs));
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
			  code_pair_t *cache = nullptr) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_sid (glyph, അക്കം_glyphs);
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_sid (glyph, അക്കം_glyphs, cache);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_sid (glyph, അക്കം_glyphs, cache);
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  ശൂന്യം collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, അക്കം_glyphs); തിരികെ;
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_glyph (sid, അക്കം_glyphs);
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_glyph (sid, അക്കം_glyphs);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_glyph (sid, അക്കം_glyphs);
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned *അക്കം_charset_entries) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്)))
      return_trace (അസത്യം);
    hb_barrier ();

    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_അക്കം_glyphs (), അക്കം_charset_entries));
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_അക്കം_glyphs (), അക്കം_charset_entries));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_അക്കം_glyphs (), അക്കം_charset_entries));
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  HBUINT8       format;
  സംഘം {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

ഘടന CFF1StringIndex : CFF1Index
{
  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം CFF1StringIndex &strings,
		  സ്ഥിരം hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely ((strings.എണ്ണം == 0) || (sidmap.length == 0)))
    {
      എങ്കിൽ (unlikely (!c->extend_min (ഇത്->എണ്ണം)))
	return_trace (അസത്യം);
      എണ്ണം = 0;
      return_trace (സത്യം);
    }

    എങ്കിൽ (unlikely (sidmap.in_error ())) return_trace (അസത്യം);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    എങ്കിൽ (unlikely (bytesArray.in_error ())) return_trace (അസത്യം);

    തർക്കം result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

ഘടന cff1_മുകൾ_dict_interp_env_t : അക്കം_interp_env_t
{
  cff1_മുകൾ_dict_interp_env_t ()
    : അക്കം_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_മുകൾ_dict_interp_env_t (സ്ഥിരം hb_ubytes_t &bytes)
    : അക്കം_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned പൂർണ്ണസംഖ്യ prev_offset;
  unsigned പൂർണ്ണസംഖ്യ last_offset;
};

ഘടന name_dict_മൂല്യങ്ങൾ_t
{
  എണ്ണൽ name_dict_val_index_t
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

      Val_എണ്ണം
  };

  ശൂന്യം init ()
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < Val_എണ്ണം; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned പൂർണ്ണസംഖ്യ& operator[] (unsigned പൂർണ്ണസംഖ്യ i)
  { പ്രതിജ്ഞാ (i < Val_എണ്ണം); തിരികെ values[i]; }

  unsigned പൂർണ്ണസംഖ്യ operator[] (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { പ്രതിജ്ഞാ (i < Val_എണ്ണം); തിരികെ values[i]; }

  സ്ഥിതം എണ്ണൽ name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      സ്ഥിരസ്ഥിതി: // can't happen - just make some compiler happy
      അവസ്ഥ OpCode_version:
	തിരികെ version;
      അവസ്ഥ OpCode_Notice:
	തിരികെ notice;
      അവസ്ഥ OpCode_Copyright:
	തിരികെ copyright;
      അവസ്ഥ OpCode_FullName:
	തിരികെ fullName;
      അവസ്ഥ OpCode_FamilyName:
	തിരികെ familyName;
      അവസ്ഥ OpCode_Weight:
	തിരികെ weight;
      അവസ്ഥ OpCode_PostScript:
	തിരികെ postscript;
      അവസ്ഥ OpCode_FontName:
	തിരികെ fontName;
      അവസ്ഥ OpCode_BaseFontName:
	തിരികെ baseFontName;
    }
  }

  unsigned പൂർണ്ണസംഖ്യ  values[Val_എണ്ണം];
};

ഘടന cff1_മുകൾ_dict_val_t : op_str_t
{
  unsigned പൂർണ്ണസംഖ്യ  last_arg_offset;
};

ഘടന cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t : മുകൾ_dict_മൂല്യങ്ങൾ_t<cff1_മുകൾ_dict_val_t>
{
  ശൂന്യം init ()
  {
    മുകൾ_dict_മൂല്യങ്ങൾ_t<cff1_മുകൾ_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_എണ്ണം = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  ശൂന്യം fini () { മുകൾ_dict_മൂല്യങ്ങൾ_t<cff1_മുകൾ_dict_val_t>::fini (); }

  തർക്കം is_CID () സ്ഥിരം
  { തിരികെ nameSIDs[name_dict_മൂല്യങ്ങൾ_t::registry] != CFF_UNDEF_SID; }

  name_dict_മൂല്യങ്ങൾ_t  nameSIDs;
  unsigned പൂർണ്ണസംഖ്യ    ros_supplement_offset;
  unsigned പൂർണ്ണസംഖ്യ    ros_supplement;
  unsigned പൂർണ്ണസംഖ്യ    cid_എണ്ണം;

  പൂർണ്ണസംഖ്യ             EncodingOffset;
  പൂർണ്ണസംഖ്യ             CharsetOffset;
  പൂർണ്ണസംഖ്യ             FDSelectOffset;
  table_info_t    privateDictInfo;
};

ഘടന cff1_മുകൾ_dict_opset_t : മുകൾ_dict_opset_t<cff1_മുകൾ_dict_val_t>
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff1_മുകൾ_dict_interp_env_t& env, cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t& dictval)
  {
    cff1_മുകൾ_dict_val_t  val {};

    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_version:
      അവസ്ഥ OpCode_Notice:
      അവസ്ഥ OpCode_Copyright:
      അവസ്ഥ OpCode_FullName:
      അവസ്ഥ OpCode_FontName:
      അവസ്ഥ OpCode_FamilyName:
      അവസ്ഥ OpCode_Weight:
      അവസ്ഥ OpCode_PostScript:
      അവസ്ഥ OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_മൂല്യങ്ങൾ_t::name_op_to_index (op)] = env.arg_അട്ടി.എടുക്കുക_uint ();
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_isFixedPitch:
      അവസ്ഥ OpCode_ItalicAngle:
      അവസ്ഥ OpCode_UnderlinePosition:
      അവസ്ഥ OpCode_UnderlineThickness:
      അവസ്ഥ OpCode_PaintType:
      അവസ്ഥ OpCode_CharstringType:
      അവസ്ഥ OpCode_UniqueID:
      അവസ്ഥ OpCode_StrokeWidth:
      അവസ്ഥ OpCode_SyntheticBase:
      അവസ്ഥ OpCode_CIDFontVersion:
      അവസ്ഥ OpCode_CIDFontRevision:
      അവസ്ഥ OpCode_CIDFontType:
      അവസ്ഥ OpCode_UIDBase:
      അവസ്ഥ OpCode_FontBBox:
      അവസ്ഥ OpCode_XUID:
      അവസ്ഥ OpCode_BaseFontBlend:
	env.clear_args ();
	നിർത്തുക;

      അവസ്ഥ OpCode_CIDCount:
	dictval.cid_എണ്ണം = env.arg_അട്ടി.എടുക്കുക_uint ();
	env.clear_args ();
	നിർത്തുക;

      അവസ്ഥ OpCode_ROS:
	dictval.ros_supplement = env.arg_അട്ടി.എടുക്കുക_uint ();
	dictval.nameSIDs[name_dict_മൂല്യങ്ങൾ_t::ordering] = env.arg_അട്ടി.എടുക്കുക_uint ();
	dictval.nameSIDs[name_dict_മൂല്യങ്ങൾ_t::registry] = env.arg_അട്ടി.എടുക്കുക_uint ();
	env.clear_args ();
	എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;
	പ്രതിജ്ഞാ (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	നിർത്തുക;

      അവസ്ഥ OpCode_Encoding:
	dictval.EncodingOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	എങ്കിൽ (unlikely (dictval.EncodingOffset == 0)) തിരികെ;
	നിർത്തുക;

      അവസ്ഥ OpCode_charset:
	dictval.CharsetOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	എങ്കിൽ (unlikely (dictval.CharsetOffset == 0)) തിരികെ;
	നിർത്തുക;

      അവസ്ഥ OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;

      അവസ്ഥ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_അട്ടി.എടുക്കുക_int ();
	dictval.privateDictInfo.size = env.arg_അട്ടി.എടുക്കുക_uint ();
	env.clear_args ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	env.last_offset = env.str_ref.get_offset ();
	മുകൾ_dict_opset_t<cff1_മുകൾ_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
	നിർത്തുക;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ഘടന cff1_font_dict_മൂല്യങ്ങൾ_t : dict_മൂല്യങ്ങൾ_t<op_str_t>
{
  ശൂന്യം init ()
  {
    dict_മൂല്യങ്ങൾ_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  ശൂന്യം fini () { dict_മൂല്യങ്ങൾ_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned പൂർണ്ണസംഖ്യ    fontName;
};

ഘടന cff1_font_dict_opset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, അക്കം_interp_env_t& env, cff1_font_dict_മൂല്യങ്ങൾ_t& dictval)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_FontName:
	dictval.fontName = env.arg_അട്ടി.എടുക്കുക_uint ();
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_FontMatrix:
      അവസ്ഥ OpCode_PaintType:
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_അട്ടി.എടുക്കുക_uint ();
	dictval.privateDictInfo.size = env.arg_അട്ടി.എടുക്കുക_uint ();
	env.clear_args ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	dict_opset_t::process_op (op, env);
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
	നിർത്തുക;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
ഘടന cff1_private_dict_മൂല്യങ്ങൾ_base_t : dict_മൂല്യങ്ങൾ_t<VAL>
{
  ശൂന്യം init ()
  {
    dict_മൂല്യങ്ങൾ_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  ശൂന്യം fini () { dict_മൂല്യങ്ങൾ_t<VAL>::fini (); }

  പൂർണ്ണസംഖ്യ                 subrsOffset;
  സ്ഥിരം CFF1Subrs    *localSubrs;
};

തരനിർവചനം cff1_private_dict_മൂല്യങ്ങൾ_base_t<op_str_t> cff1_private_dict_മൂല്യങ്ങൾ_subset_t;
തരനിർവചനം cff1_private_dict_മൂല്യങ്ങൾ_base_t<അക്കം_dict_val_t> cff1_private_dict_മൂല്യങ്ങൾ_t;

ഘടന cff1_private_dict_opset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, അക്കം_interp_env_t& env, cff1_private_dict_മൂല്യങ്ങൾ_t& dictval)
  {
    അക്കം_dict_val_t val;
    val.init ();

    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ Op_Code_Blue_മൂല്യങ്ങൾ:
      അവസ്ഥ OpCode_OtherBlues:
      അവസ്ഥ OpCode_FamilyBlues:
      അവസ്ഥ OpCode_FamilyOtherBlues:
      അവസ്ഥ OpCode_StemSnapH:
      അവസ്ഥ OpCode_StemSnapV:
      അവസ്ഥ OpCode_StdHW:
      അവസ്ഥ OpCode_StdVW:
      അവസ്ഥ OpCode_BlueScale:
      അവസ്ഥ OpCode_BlueShift:
      അവസ്ഥ OpCode_BlueFuzz:
      അവസ്ഥ OpCode_ForceBold:
      അവസ്ഥ OpCode_LanguageGroup:
      അവസ്ഥ OpCode_ExpansionFactor:
      അവസ്ഥ OpCode_initialRandomSeed:
      അവസ്ഥ OpCode_defaultWidthX:
      അവസ്ഥ OpCode_nominalWidthX:
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_Subrs:
	dictval.subrsOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	dict_opset_t::process_op (op, env);
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
	നിർത്തുക;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ഘടന cff1_private_dict_opset_subset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, അക്കം_interp_env_t& env, cff1_private_dict_മൂല്യങ്ങൾ_subset_t& dictval)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ Op_Code_Blue_മൂല്യങ്ങൾ:
      അവസ്ഥ OpCode_OtherBlues:
      അവസ്ഥ OpCode_FamilyBlues:
      അവസ്ഥ OpCode_FamilyOtherBlues:
      അവസ്ഥ OpCode_StemSnapH:
      അവസ്ഥ OpCode_StemSnapV:
      അവസ്ഥ OpCode_StdHW:
      അവസ്ഥ OpCode_StdVW:
      അവസ്ഥ OpCode_BlueScale:
      അവസ്ഥ OpCode_BlueShift:
      അവസ്ഥ OpCode_BlueFuzz:
      അവസ്ഥ OpCode_ForceBold:
      അവസ്ഥ OpCode_LanguageGroup:
      അവസ്ഥ OpCode_ExpansionFactor:
      അവസ്ഥ OpCode_initialRandomSeed:
      അവസ്ഥ OpCode_defaultWidthX:
      അവസ്ഥ OpCode_nominalWidthX:
	env.clear_args ();
	നിർത്തുക;

      അവസ്ഥ OpCode_Subrs:
	dictval.subrsOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	dict_opset_t::process_op (op, env);
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
	നിർത്തുക;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    dictval.add_op (op, env.str_ref);
  }
};

തരനിർവചനം dict_interpreter_t<cff1_മുകൾ_dict_opset_t, cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t, cff1_മുകൾ_dict_interp_env_t> cff1_മുകൾ_dict_interpreter_t;
തരനിർവചനം dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_മൂല്യങ്ങൾ_t> cff1_font_dict_interpreter_t;

തരനിർവചനം CFF1Index CFF1NameIndex;
തരനിർവചനം CFF1Index CFF1_മുകൾ_Dict_Index;

ഘടന cff1_font_dict_മൂല്യങ്ങൾ_mod_t
{
  cff1_font_dict_മൂല്യങ്ങൾ_mod_t() { init (); }

  ശൂന്യം init () { init ( &Null (cff1_font_dict_മൂല്യങ്ങൾ_t), CFF_UNDEF_SID ); }

  ശൂന്യം init (സ്ഥിരം cff1_font_dict_മൂല്യങ്ങൾ_t *base_,
	     unsigned പൂർണ്ണസംഖ്യ fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_എണ്ണം () സ്ഥിരം { തിരികെ base->get_എണ്ണം (); }

  സ്ഥിരം op_str_t &operator [] (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം { തിരികെ (*base)[i]; }

  സ്ഥിരം cff1_font_dict_മൂല്യങ്ങൾ_t    *base;
  table_info_t		   privateDictInfo;
  unsigned പൂർണ്ണസംഖ്യ		fontName;
};

ഘടന CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  തർക്കം serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { തിരികെ FDArray<HBUINT16>::serialize<cff1_font_dict_മൂല്യങ്ങൾ_mod_t, cff1_font_dict_മൂല്യങ്ങൾ_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

നാമാകാശ OT {

using നാമാകാശ CFF;

ഘടന cff1
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  ഘടന accelerator_templ_t
  {
    സ്ഥിതം constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      എങ്കിൽ (!face) തിരികെ;

      മുകൾ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      ഇത്->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (ഇത്->blob);
      sc.start_processing ();

      സ്ഥിരം OT::cff1 *cff = ഇത്->blob->template as<OT::cff1> ();

      എങ്കിൽ (cff == &Null (OT::cff1))
        പോകുക fail;

      nameIndex = &cff->nameIndex (cff);
      എങ്കിൽ ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        പോകുക fail;
      hb_barrier ();

      മുകൾ_Dict_Index = &StructAtOffsetOrNull<CFF1_മുകൾ_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      എങ്കിൽ (മുകൾ_Dict_Index == &Null (CFF1_മുകൾ_Dict_Index) || (മുകൾ_Dict_Index->എണ്ണം == 0))
        പോകുക fail;
      hb_barrier ();

      { /* parse top dict */
	സ്ഥിരം hb_ubytes_t മുകൾ_Dict_Str = (*മുകൾ_Dict_Index)[0];
	എങ്കിൽ (unlikely (!മുകൾ_Dict_Str.sanitize (&sc)))   പോകുക fail;
	hb_barrier ();
	cff1_മുകൾ_dict_interp_env_t env (മുകൾ_Dict_Str);
	cff1_മുകൾ_dict_interpreter_t മുകൾ_interp (env);
	എങ്കിൽ (unlikely (!മുകൾ_interp.interpret (മുകൾ_Dict_2)))   പോകുക fail;
      }

      എങ്കിൽ (is_predef_charset ())
	charset = &Null (Charset);
      അല്ലെങ്കിൽ
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, മുകൾ_Dict_2.CharsetOffset, sc, &അക്കം_charset_entries);
	എങ്കിൽ (unlikely (charset == &Null (Charset)))   പോകുക fail;
      }

      fd_എണ്ണം = 1;
      എങ്കിൽ (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, മുകൾ_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, മുകൾ_Dict_2.FDSelectOffset, sc, fdArray->എണ്ണം);
	എങ്കിൽ (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  പോകുക fail;

	fd_എണ്ണം = fdArray->എണ്ണം;
      }
      അല്ലെങ്കിൽ
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      എങ്കിൽ (is_CID ())
      {
	എങ്കിൽ (unlikely (charset == &Null (Charset)))   പോകുക fail;
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, മുകൾ_Dict_2.EncodingOffset, sc);
	  എങ്കിൽ (unlikely (encoding == &Null (Encoding)))   പോകുക fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (മുകൾ_Dict_Index, മുകൾ_Dict_Index->get_size (), sc);
      എങ്കിൽ (stringIndex == &Null (CFF1StringIndex))
        പോകുക fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, മുകൾ_Dict_2.charStringsOffset, sc);
      എങ്കിൽ (charStrings == &Null (CFF1CharStrings))
        പോകുക fail;

      അക്കം_glyphs = charStrings->എണ്ണം;
      എങ്കിൽ (അക്കം_glyphs != sc.get_അക്കം_glyphs ())
        പോകുക fail;

      എങ്കിൽ (unlikely (!privateDicts.resize (fd_എണ്ണം)))
        പോകുക fail;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < fd_എണ്ണം; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      എങ്കിൽ (is_CID ())
      {
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < fd_എണ്ണം; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  എങ്കിൽ (unlikely (!fontDictStr.sanitize (&sc)))   പോകുക fail;
	  hb_barrier ();
	  cff1_font_dict_മൂല്യങ്ങൾ_t *font;
	  cff1_മുകൾ_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  എങ്കിൽ (unlikely (fontDicts.in_error ()))   പോകുക fail;

	  font->init ();
	  എങ്കിൽ (unlikely (!font_interp.interpret (*font)))   പോകുക fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  സ്ഥിരം hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  എങ്കിൽ (unlikely (font->privateDictInfo.size &&
			privDictStr == (സ്ഥിരം unsigned അക്ഷരം *) &Null (UnsizedByteStr))) പോകുക fail;
	  അക്കം_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  എങ്കിൽ (unlikely (!priv_interp.interpret (*priv)))   പോകുക fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      അല്ലെങ്കിൽ  /* non-CID */
      {
	cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t *font = &മുകൾ_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	സ്ഥിരം hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	എങ്കിൽ (font->privateDictInfo.size &&
	    unlikely (privDictStr == (സ്ഥിരം unsigned അക്ഷരം *) &Null (UnsizedByteStr))) പോകുക fail;
	അക്കം_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	എങ്കിൽ (unlikely (!priv_interp.interpret (*priv)))   പോകുക fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      തിരികെ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    ശൂന്യം _fini ()
    {
      sc.end_processing ();
      മുകൾ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () സ്ഥിരം { തിരികെ blob; }

    തർക്കം is_valid () സ്ഥിരം { തിരികെ blob; }
    തർക്കം   is_CID () സ്ഥിരം { തിരികെ മുകൾ_Dict_2.is_CID (); }

    തർക്കം is_predef_charset () സ്ഥിരം { തിരികെ മുകൾ_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned പൂർണ്ണസംഖ്യ std_code_to_glyph (hb_codepoint_t code) സ്ഥിരം
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      എങ്കിൽ (unlikely (sid == CFF_UNDEF_SID))
	തിരികെ 0;

      എങ്കിൽ (charset != &Null (Charset))
	തിരികെ charset->get_glyph (sid, അക്കം_glyphs);
      അല്ലെങ്കിൽ എങ്കിൽ ((മുകൾ_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) തിരികെ sid;
      തിരികെ 0;
    }

    തർക്കം is_predef_encoding () സ്ഥിരം { തിരികെ മുകൾ_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) സ്ഥിരം
    {
      എങ്കിൽ (encoding != &Null (Encoding))
	തിരികെ encoding->get_code (glyph);
      അല്ലെങ്കിൽ
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	എങ്കിൽ (sid == 0) തിരികെ 0;
	hb_codepoint_t code = 0;
	തിരഞ്ഞെടുപ്പ് (മുകൾ_Dict_2.EncodingOffset)
	{
	അവസ്ഥ StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  നിർത്തുക;
	അവസ്ഥ ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
	  നിർത്തുക;
	}
	തിരികെ code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () സ്ഥിരം
    {
      എങ്കിൽ (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (വലുപ്പം (glyph_to_sid_map_t));
	എങ്കിൽ (unlikely (!mapping)) തിരികെ nullptr;
	mapping = പുതിയ (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, അക്കം_glyphs);
	തിരികെ mapping;
      }
      അല്ലെങ്കിൽ
	തിരികെ nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) സ്ഥിരം
    {
      എങ്കിൽ (charset != &Null (Charset))
	തിരികെ charset->get_sid (glyph, അക്കം_glyphs, cache);
      അല്ലെങ്കിൽ
      {
	hb_codepoint_t sid = 0;
	തിരഞ്ഞെടുപ്പ് (മുകൾ_Dict_2.CharsetOffset)
	{
	  അവസ്ഥ ISOAdobeCharset:
	    എങ്കിൽ (glyph <= 228 /*zcaron*/) sid = glyph;
	    നിർത്തുക;
	  അവസ്ഥ ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    നിർത്തുക;
	  അവസ്ഥ ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    നിർത്തുക;
	  സ്ഥിരസ്ഥിതി:
	    നിർത്തുക;
	}
	തിരികെ sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) സ്ഥിരം
    {
      എങ്കിൽ (charset != &Null (Charset))
	തിരികെ charset->get_glyph (sid, അക്കം_glyphs);
      അല്ലെങ്കിൽ
      {
	hb_codepoint_t glyph = 0;
	തിരഞ്ഞെടുപ്പ് (മുകൾ_Dict_2.CharsetOffset)
	{
	  അവസ്ഥ ISOAdobeCharset:
	    എങ്കിൽ (sid <= 228 /*zcaron*/) glyph = sid;
	    നിർത്തുക;
	  അവസ്ഥ ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    നിർത്തുക;
	  അവസ്ഥ ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    നിർത്തുക;
	  സ്ഥിരസ്ഥിതി:
	    നിർത്തുക;
	}
	തിരികെ glyph;
      }
    }

    സംരക്ഷിത:
    hb_sanitize_context_t   sc;

    പൊതു:
    hb_blob_t               *blob = nullptr;
    സ്ഥിരം Encoding	    *encoding = nullptr;
    സ്ഥിരം Charset	    *charset = nullptr;
    സ്ഥിരം CFF1NameIndex     *nameIndex = nullptr;
    സ്ഥിരം CFF1_മുകൾ_Dict_Index  *മുകൾ_Dict_Index = nullptr;
    സ്ഥിരം CFF1StringIndex   *stringIndex = nullptr;
    സ്ഥിരം CFF1Subrs	    *globalSubrs = nullptr;
    സ്ഥിരം CFF1CharStrings   *charStrings = nullptr;
    സ്ഥിരം CFF1FDArray       *fdArray = nullptr;
    സ്ഥിരം CFF1FDSelect      *fdSelect = nullptr;
    unsigned പൂർണ്ണസംഖ്യ	     fd_എണ്ണം = 0;

    cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t   മുകൾ_Dict_2;
    hb_vector_t<cff1_font_dict_മൂല്യങ്ങൾ_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned പൂർണ്ണസംഖ്യ	     അക്കം_glyphs = 0;
    unsigned പൂർണ്ണസംഖ്യ	     അക്കം_charset_entries = 0;
  };

  ഘടന accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_മൂല്യങ്ങൾ_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      എങ്കിൽ (!is_valid ()) തിരികെ;
      എങ്കിൽ (is_CID ()) തിരികെ;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      എങ്കിൽ (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    തർക്കം get_glyph_name (hb_codepoint_t glyph,
			 അക്ഷരം *buf, unsigned പൂർണ്ണസംഖ്യ buf_len) സ്ഥിരം
    {
      എങ്കിൽ (unlikely (glyph >= അക്കം_glyphs)) തിരികെ അസത്യം;
      എങ്കിൽ (unlikely (!is_valid ())) തിരികെ അസത്യം;
      എങ്കിൽ (is_CID()) തിരികെ അസത്യം;
      എങ്കിൽ (unlikely (!buf_len)) തിരികെ സത്യം;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      സ്ഥിരം അക്ഷരം *str;
      size_t str_len;
      എങ്കിൽ (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      അല്ലെങ്കിൽ
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (സ്ഥിരം അക്ഷരം *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      എങ്കിൽ (!str_len) തിരികെ അസത്യം;
      unsigned പൂർണ്ണസംഖ്യ len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (സ്ഥിരം അക്ഷരം*)str, len);
      buf[len] = '\0';
      തിരികെ സത്യം;
    }

    തർക്കം get_glyph_from_name (സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ len,
			      hb_codepoint_t *glyph) സ്ഥിരം
    {
      എങ്കിൽ (unlikely (!is_valid ())) തിരികെ അസത്യം;
      എങ്കിൽ (is_CID()) തിരികെ അസത്യം;
      എങ്കിൽ (len < 0) len = strlen (name);
      എങ്കിൽ (unlikely (!len)) തിരികെ അസത്യം;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      എങ്കിൽ (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, വലുപ്പം (hb_sorted_vector_t<gname_t>));
	എങ്കിൽ (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  വേണ്ടി (hb_codepoint_t gid = 0; gid < അക്കം_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    എങ്കിൽ (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    അല്ലെങ്കിൽ
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((സ്ഥിരം അക്ഷരം*) ustr.arrayZ, ustr.length);
	    }
	    എങ്കിൽ (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	എങ്കിൽ (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  എങ്കിൽ (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  പോകുക retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      സ്ഥിരം gname_t *gname = names ? names->bsearch (key) : nullptr;
      എങ്കിൽ (!gname) തിരികെ അസത്യം;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      എങ്കിൽ (!gid && gname->sid) തിരികെ അസത്യം;
      *glyph = gid;
      തിരികെ സത്യം;
    }

    HB_INTERNAL തർക്കം get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) സ്ഥിരം;
    HB_INTERNAL തർക്കം get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) സ്ഥിരം;

    സ്വകാര്യ:
    ഘടന gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *a_, സ്ഥിരം ശൂന്യം *b_)
      {
	സ്ഥിരം gname_t *a = (സ്ഥിരം gname_t *)a_;
	സ്ഥിരം gname_t *b = (സ്ഥിരം gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	പൂർണ്ണസംഖ്യ ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	എങ്കിൽ (ret) തിരികെ ret;
	തിരികെ a->name.length - b->name.length;
      }

      പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം gname_t &a) സ്ഥിരം { തിരികെ cmp (&a, ഇത്); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    തരനിർവചനം accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_മൂല്യങ്ങൾ_t> SUPER;
  };

  ഘടന accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_മൂല്യങ്ങൾ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      എങ്കിൽ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം;
    HB_INTERNAL തർക്കം serialize (hb_serialize_context_t *c,
				ഘടന cff1_subset_plan &plan) സ്ഥിരം;
    HB_INTERNAL തർക്കം get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) സ്ഥിരം;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    തരനിർവചനം accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_മൂല്യങ്ങൾ_subset_t> SUPER;
  };

  സംരക്ഷിത:
  HB_INTERNAL സ്ഥിതം hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL സ്ഥിതം hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL സ്ഥിതം hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL സ്ഥിതം hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL സ്ഥിതം hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL സ്ഥിതം hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL സ്ഥിതം hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  പൊതു:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

ഘടന cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  ശൂന്യം depend (hb_depend_data_builder_t *builder) സ്ഥിരം
  {
    എങ്കിൽ (!is_valid ()) തിരികെ;
    വേണ്ടി (hb_codepoint_t gid = 0; gid < അക്കം_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      എങ്കിൽ (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
