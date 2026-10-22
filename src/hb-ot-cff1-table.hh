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

#ଅନ୍ତର୍ଭାଵ "hb-ot-cff-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-depend-data.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-draw.hh"
#ଅନ୍ତର୍ଭାଵ "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#ଅନ୍ତର୍ଭାଵ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

ନାମାକାଶ CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

ଗଣନା EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
ଗଣନା CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

ପ୍ରକାରବ୍ୟାଖ୍ୟା CFF1Index          CFF1CharStrings;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Subrs<HBUINT16>    CFF1Subrs;

ସଂରଚନା CFF1FDSelect : FDSelect {};

/* Encoding */
ସଂରଚନା Encoding0 {
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ଅଚଳ
  {
    ପ୍ରତିଜ୍ଞା (glyph > 0);
    glyph--;
    ଯଦି (glyph < nCodes ())
    {
      ଫେରନ୍ତୁ (hb_codepoint_t)codes[glyph];
    }
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { ଫେରନ୍ତୁ codes.len; }
  HBUINT8 nCodes () ଅଚଳ { ଫେରନ୍ତୁ codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

ସଂରଚନା Encoding1_Range {
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  HBUINT8   first;
  HBUINT8   n_ବାମ;

  DEFINE_SIZE_STATIC (2);
};

ସଂରଚନା Encoding1 {
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ଅଚଳ
  {
    /* TODO: Add cache like get_sid. */
    ପ୍ରତିଜ୍ଞା (glyph > 0);
    glyph--;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nRanges (); i++)
    {
      ଯଦି (glyph <= ranges[i].n_ବାମ)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	ଫେରନ୍ତୁ (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_ବାମ + 1);
    }
    ଫେରନ୍ତୁ CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { ଫେରନ୍ତୁ ranges.len; }
  HBUINT8 nRanges () ଅଚଳ { ଫେରନ୍ତୁ ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

ସଂରଚନା SuppEncoding {
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

ସଂରଚନା CFF1SuppEncData {
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (supps.sanitize (c));
  }

  ଶୂନ୍ୟ get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ଅଚଳ
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nSups (); i++)
      ଯଦି (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { ଫେରନ୍ତୁ supps.len; }
  HBUINT8 nSups () ଅଚଳ { ଫେରନ୍ତୁ supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

ସଂରଚନା Encoding
{
  /* serialize a fullset Encoding */
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ Encoding &src)
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା enc_ଗଣନା,
		  ଅଚଳ hb_vector_t<code_pair_t>& code_ranges,
		  ଅଚଳ hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (ଏହି);
    Encoding *dest = c->extend_min (ଏହି);
    ଯଦି (unlikely (!dest)) return_trace (ମିଥ୍ୟା);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    ବାଛ (format) {
    ସ୍ଥିତି 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_ଗଣନା);
      ଯଦି (unlikely (!fmt0)) return_trace (ମିଥ୍ୟା);
      fmt0->nCodes () = enc_ଗଣନା;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph = 0;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା left = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	ଯଦି (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (ମିଥ୍ୟା);
      }
    }
    ଭାଙ୍ଗ;

    ସ୍ଥିତି 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      ଯଦି (unlikely (!fmt1)) return_trace (ମିଥ୍ୟା);
      fmt1->nRanges () = code_ranges.length;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < code_ranges.length; i++)
      {
	ଯଦି (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (ମିଥ୍ୟା);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_ବାମ = code_ranges[i].glyph;
      }
    }
    ଭାଙ୍ଗ;

    }

    ଯଦି (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      ଯଦି (unlikely (!suppData)) return_trace (ମିଥ୍ୟା);
      suppData->nSups () = supp_codes.length;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (ସତ୍ୟ);
  }

  size_t get_size () ଅଚଳ
  {
    size_t size = min_size;
    ବାଛ (table_format ())
    {
    ସ୍ଥିତି 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); ଭାଙ୍ଗ;
    ସ୍ଥିତି 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); ଭାଙ୍ଗ;
    }
    ଯଦି (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    ଫେରନ୍ତୁ size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ଅଚଳ
  {
    ବାଛ (table_format ())
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_code (glyph);
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_code (glyph);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  uint8_t table_format () ଅଚଳ { ଫେରନ୍ତୁ format & 0x7F; }
  ତାର୍କିକ  has_supplement () ଅଚଳ { ଫେରନ୍ତୁ format & 0x80; }

  ଶୂନ୍ୟ get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ଅଚଳ
  {
    codes.clear ();
    ଯଦି (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି)))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ବାଛ (table_format ())
    {
    ସ୍ଥିତି 0: hb_barrier (); ଯଦି (unlikely (!u.format0.sanitize (c))) { return_trace (ମିଥ୍ୟା); } ଭାଙ୍ଗ;
    ସ୍ଥିତି 1: hb_barrier (); ଯଦି (unlikely (!u.format1.sanitize (c))) { return_trace (ମିଥ୍ୟା); } ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  ସୁରକ୍ଷିତ:
  ଅଚଳ CFF1SuppEncData &suppEncData () ଅଚଳ
  {
    ବାଛ (table_format ())
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ Null (CFF1SuppEncData);
    }
  }

  ସର୍ବସାଧାରଣ:
  HBUINT8	format;
  ସଂଘ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
ସଂରଚନା Charset0
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs, unsigned *ଅଙ୍କ_charset_entries) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (ଅଙ୍କ_charset_entries) *ଅଙ୍କ_charset_entries = ଅଙ୍କ_glyphs;
    return_trace (sids.sanitize (c, ଅଙ୍କ_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଯଦି (unlikely (glyph >= ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ 0;
    ଯଦି (unlikely (glyph == 0))
      ଫେରନ୍ତୁ 0;
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ sids[glyph - 1];
  }

  ଶୂନ୍ୟ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଯଦି (!mapping->resize_dirty  (ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ;
    ପାଇଁ (hb_codepoint_t gid = 1; gid < ଅଙ୍କ_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଯଦି (sid == 0)
      ଫେରନ୍ତୁ 0;

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph = 1; glyph < ଅଙ୍କ_glyphs; glyph++)
    {
      ଯଦି (sids[glyph-1] == sid)
	ଫେରନ୍ତୁ glyph;
    }
    ଫେରନ୍ତୁ 0;
  }

  ସ୍ଥିର size_t get_size (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs)
  {
    ପ୍ରତିଜ୍ଞା (ଅଙ୍କ_glyphs > 0);
    ଫେରନ୍ତୁ UnsizedArrayOf<HBUINT16>::get_size (ଅଙ୍କ_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
ସଂରଚନା Charset_Range {
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  HBUINT16  first;
  TYPE      n_ବାମ;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
ସଂରଚନା Charset1_2 {
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs, unsigned *ଅଙ୍କ_charset_entries) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଅଙ୍କ_glyphs--;
    unsigned i;
    ପାଇଁ (i = 0; ଅଙ୍କ_glyphs > 0; i++)
    {
      ଯଦି (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (ଅଙ୍କ_glyphs >= ranges[i].n_ବାମ + 1))))
	return_trace (ମିଥ୍ୟା);
      ଅଙ୍କ_glyphs -= (ranges[i].n_ବାମ + 1);
    }
    ଯଦି (ଅଙ୍କ_charset_entries)
      *ଅଙ୍କ_charset_entries = i;
    return_trace (ସତ୍ୟ);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned ଅଙ୍କ_glyphs,
			  code_pair_t *cache = nullptr) ଅଚଳ
  {
    ଯଦି (unlikely (glyph >= ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    ଯଦି (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (unlikely (glyph == 0)) ଫେରନ୍ତୁ 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    ପାଇଁ (;; i++)
    {
      unsigned ଗଣନା_2 = ranges[i].n_ବାମ;
      ଯଦି (glyph <= ଗଣନା_2)
      {
        ଯଦି (cache)
	  *cache = {i, start_glyph};
	ଫେରନ୍ତୁ ranges[i].first + glyph;
      }
      ଗଣନା_2++;
      start_glyph += ଗଣନା_2;
      glyph -= ଗଣନା_2;
    }

    ଫେରନ୍ତୁ 0;
  }

  ଶୂନ୍ୟ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଯଦି (!mapping->resize_dirty  (ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ;

    hb_codepoint_t gid = 1;
    ଯଦି (gid >= ଅଙ୍କ_glyphs)
      ଫେରନ୍ତୁ;

    ପାଇଁ (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned ଗଣନା_2 = ranges[i].n_ବାମ + 1;
      unsigned last = gid + ଗଣନା_2;
      ପାଇଁ (unsigned j = 0; j < ଗଣନା_2; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      ଯଦି (gid >= ଅଙ୍କ_glyphs)
        ଭାଙ୍ଗ;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଯଦି (sid == 0) ଫେରନ୍ତୁ 0;
    hb_codepoint_t  glyph = 1;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;; i++)
    {
      ଯଦି (glyph >= ଅଙ୍କ_glyphs)
	ଫେରନ୍ତୁ 0;
      ଯଦି ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_ବାମ))
	ଫେରନ୍ତୁ glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_ବାମ + 1);
    }

    ଫେରନ୍ତୁ 0;
  }

  size_t get_size (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଅଙ୍କ_glyphs;
    unsigned ଅଙ୍କ_ranges = 0;

    ପ୍ରତିଜ୍ଞା (glyph > 0);
    glyph--;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_ବାମ + 1);
      ଅଙ୍କ_ranges++;
    }

    ଫେରନ୍ତୁ get_size_for_ranges (ଅଙ୍କ_ranges);
  }

  ସ୍ଥିର size_t get_size_for_ranges (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ranges)
  {
    ଫେରନ୍ତୁ UnsizedArrayOf<Charset_Range<TYPE> >::get_size (ଅଙ୍କ_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା Charset1_2<HBUINT8>     Charset1;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Charset1_2<HBUINT16>    Charset2;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Charset_Range<HBUINT8>  Charset1_Range;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Charset_Range<HBUINT16> Charset2_Range;

ସଂରଚନା Charset
{
  /* serialize a fullset Charset */
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ Charset &src, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs)
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed ((ଅଚଳ ଅକ୍ଷର *) &src, src.get_size (ଅଙ୍କ_glyphs)));
  }

  /* serialize a subset Charset */
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
		  ଅଚଳ hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (ଏହି);
    Charset *dest = c->extend_min (ଏହି);
    ଯଦି (unlikely (!dest)) return_trace (ମିଥ୍ୟା);
    dest->format = format;
    ବାଛ (format)
    {
    ସ୍ଥିତି 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (ଅଙ୍କ_glyphs), ମିଥ୍ୟା);
      ଯଦି (unlikely (!fmt0)) return_trace (ମିଥ୍ୟା);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph = 0;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା left = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    ଭାଙ୍ଗ;

    ସ୍ଥିତି 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), ମିଥ୍ୟା);
      ଯଦି (unlikely (!fmt1)) return_trace (ମିଥ୍ୟା);
      hb_codepoint_t all_glyphs = 0;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_ବାମ = _.glyph;
      }
      ଯଦି (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (ମିଥ୍ୟା);
    }
    ଭାଙ୍ଗ;

    ସ୍ଥିତି 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), ମିଥ୍ୟା);
      ଯଦି (unlikely (!fmt2)) return_trace (ମିଥ୍ୟା);
      hb_codepoint_t all_glyphs = 0;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_ବାମ = _.glyph;
      }
      ଯଦି (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (ମିଥ୍ୟା);
    }
    ଭାଙ୍ଗ;

    }
    return_trace (ସତ୍ୟ);
  }

  size_t get_size (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ hb_unsigned_add_saturate (min_size, u.format0.get_size (ଅଙ୍କ_glyphs));
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ hb_unsigned_add_saturate (min_size, u.format1.get_size (ଅଙ୍କ_glyphs));
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ hb_unsigned_add_saturate (min_size, u.format2.get_size (ଅଙ୍କ_glyphs));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
			  code_pair_t *cache = nullptr) ଅଚଳ
  {
    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_sid (glyph, ଅଙ୍କ_glyphs);
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_sid (glyph, ଅଙ୍କ_glyphs, cache);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_sid (glyph, ଅଙ୍କ_glyphs, cache);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  ଶୂନ୍ୟ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_glyph (sid, ଅଙ୍କ_glyphs);
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_glyph (sid, ଅଙ୍କ_glyphs);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_glyph (sid, ଅଙ୍କ_glyphs);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned *ଅଙ୍କ_charset_entries) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି)))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_ଅଙ୍କ_glyphs (), ଅଙ୍କ_charset_entries));
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_ଅଙ୍କ_glyphs (), ଅଙ୍କ_charset_entries));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_ଅଙ୍କ_glyphs (), ଅଙ୍କ_charset_entries));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  HBUINT8       format;
  ସଂଘ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

ସଂରଚନା CFF1StringIndex : CFF1Index
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ CFF1StringIndex &strings,
		  ଅଚଳ hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely ((strings.ଗଣନା_2 == 0) || (sidmap.length == 0)))
    {
      ଯଦି (unlikely (!c->extend_min (ଏହି->ଗଣନା_2)))
	return_trace (ମିଥ୍ୟା);
      ଗଣନା_2 = 0;
      return_trace (ସତ୍ୟ);
    }

    ଯଦି (unlikely (sidmap.in_error ())) return_trace (ମିଥ୍ୟା);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    ଯଦି (unlikely (bytesArray.in_error ())) return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

ସଂରଚନା cff1_ଶୀର୍ଷ_dict_interp_env_t : ଅଙ୍କ_interp_env_t
{
  cff1_ଶୀର୍ଷ_dict_interp_env_t ()
    : ଅଙ୍କ_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_ଶୀର୍ଷ_dict_interp_env_t (ଅଚଳ hb_ubytes_t &bytes)
    : ଅଙ୍କ_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_offset;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_offset;
};

ସଂରଚନା name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t
{
  ଗଣନା name_dict_val_index_t
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

      Val_ଗଣନା
  };

  ଶୂନ୍ୟ init ()
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < Val_ଗଣନା; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା& operator[] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  { ପ୍ରତିଜ୍ଞା (i < Val_ଗଣନା); ଫେରନ୍ତୁ values[i]; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator[] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ପ୍ରତିଜ୍ଞା (i < Val_ଗଣନା); ଫେରନ୍ତୁ values[i]; }

  ସ୍ଥିର ଗଣନା name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    ବାଛ (op) {
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: // can't happen - just make some compiler happy
      ସ୍ଥିତି OpCode_version:
	ଫେରନ୍ତୁ version;
      ସ୍ଥିତି OpCode_Notice:
	ଫେରନ୍ତୁ notice;
      ସ୍ଥିତି OpCode_Copyright:
	ଫେରନ୍ତୁ copyright;
      ସ୍ଥିତି OpCode_FullName:
	ଫେରନ୍ତୁ fullName;
      ସ୍ଥିତି OpCode_FamilyName:
	ଫେରନ୍ତୁ familyName;
      ସ୍ଥିତି OpCode_Weight:
	ଫେରନ୍ତୁ weight;
      ସ୍ଥିତି OpCode_PostScript:
	ଫେରନ୍ତୁ postscript;
      ସ୍ଥିତି OpCode_FontName:
	ଫେରନ୍ତୁ fontName;
      ସ୍ଥିତି OpCode_BaseFontName:
	ଫେରନ୍ତୁ baseFontName;
    }
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  values[Val_ଗଣନା];
};

ସଂରଚନା cff1_ଶୀର୍ଷ_dict_val_t : op_str_t
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  last_arg_offset;
};

ସଂରଚନା cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t : ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<cff1_ଶୀର୍ଷ_dict_val_t>
{
  ଶୂନ୍ୟ init ()
  {
    ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<cff1_ଶୀର୍ଷ_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_ଗଣନା = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  ଶୂନ୍ୟ fini () { ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<cff1_ଶୀର୍ଷ_dict_val_t>::fini (); }

  ତାର୍କିକ is_CID () ଅଚଳ
  { ଫେରନ୍ତୁ nameSIDs[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::registry] != CFF_UNDEF_SID; }

  name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t  nameSIDs;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    ros_supplement_offset;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    ros_supplement;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    cid_ଗଣନା;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା             EncodingOffset;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା             CharsetOffset;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା             FDSelectOffset;
  table_info_t    privateDictInfo;
};

ସଂରଚନା cff1_ଶୀର୍ଷ_dict_opset_t : ଶୀର୍ଷ_dict_opset_t<cff1_ଶୀର୍ଷ_dict_val_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff1_ଶୀର୍ଷ_dict_interp_env_t& env, cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t& dictval)
  {
    cff1_ଶୀର୍ଷ_dict_val_t  val {};

    ବାଛ (op) {
      ସ୍ଥିତି OpCode_version:
      ସ୍ଥିତି OpCode_Notice:
      ସ୍ଥିତି OpCode_Copyright:
      ସ୍ଥିତି OpCode_FullName:
      ସ୍ଥିତି OpCode_FontName:
      ସ୍ଥିତି OpCode_FamilyName:
      ସ୍ଥିତି OpCode_Weight:
      ସ୍ଥିତି OpCode_PostScript:
      ସ୍ଥିତି OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::name_op_to_index (op)] = env.arg_ଗଦା.କାଢ଼_uint ();
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_isFixedPitch:
      ସ୍ଥିତି OpCode_ItalicAngle:
      ସ୍ଥିତି OpCode_UnderlinePosition:
      ସ୍ଥିତି OpCode_UnderlineThickness:
      ସ୍ଥିତି OpCode_PaintType:
      ସ୍ଥିତି OpCode_CharstringType:
      ସ୍ଥିତି OpCode_UniqueID:
      ସ୍ଥିତି OpCode_StrokeWidth:
      ସ୍ଥିତି OpCode_SyntheticBase:
      ସ୍ଥିତି OpCode_CIDFontVersion:
      ସ୍ଥିତି OpCode_CIDFontRevision:
      ସ୍ଥିତି OpCode_CIDFontType:
      ସ୍ଥିତି OpCode_UIDBase:
      ସ୍ଥିତି OpCode_FontBBox:
      ସ୍ଥିତି OpCode_XUID:
      ସ୍ଥିତି OpCode_BaseFontBlend:
	env.clear_args ();
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_CIDCount:
	dictval.cid_ଗଣନା = env.arg_ଗଦା.କାଢ଼_uint ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_ROS:
	dictval.ros_supplement = env.arg_ଗଦା.କାଢ଼_uint ();
	dictval.nameSIDs[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::ordering] = env.arg_ଗଦା.କାଢ଼_uint ();
	dictval.nameSIDs[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::registry] = env.arg_ଗଦା.କାଢ଼_uint ();
	env.clear_args ();
	ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;
	ପ୍ରତିଜ୍ଞା (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_Encoding:
	dictval.EncodingOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଯଦି (unlikely (dictval.EncodingOffset == 0)) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_charset:
	dictval.CharsetOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଯଦି (unlikely (dictval.CharsetOffset == 0)) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ଗଦା.କାଢ଼_int ();
	dictval.privateDictInfo.size = env.arg_ଗଦା.କାଢ଼_uint ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	env.last_offset = env.str_ref.get_offset ();
	ଶୀର୍ଷ_dict_opset_t<cff1_ଶୀର୍ଷ_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ସଂରଚନା cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t : dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<op_str_t>
{
  ଶୂନ୍ୟ init ()
  {
    dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  ଶୂନ୍ୟ fini () { dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    fontName;
};

ସଂରଚନା cff1_font_dict_opset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, ଅଙ୍କ_interp_env_t& env, cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t& dictval)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_FontName:
	dictval.fontName = env.arg_ଗଦା.କାଢ଼_uint ();
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_FontMatrix:
      ସ୍ଥିତି OpCode_PaintType:
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ଗଦା.କାଢ଼_uint ();
	dictval.privateDictInfo.size = env.arg_ଗଦା.କାଢ଼_uint ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	dict_opset_t::process_op (op, env);
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
ସଂରଚନା cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_base_t : dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<VAL>
{
  ଶୂନ୍ୟ init ()
  {
    dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  ଶୂନ୍ୟ fini () { dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<VAL>::fini (); }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା                 subrsOffset;
  ଅଚଳ CFF1Subrs    *localSubrs;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_base_t<op_str_t> cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_subset_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_base_t<ଅଙ୍କ_dict_val_t> cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t;

ସଂରଚନା cff1_private_dict_opset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, ଅଙ୍କ_interp_env_t& env, cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t& dictval)
  {
    ଅଙ୍କ_dict_val_t val;
    val.init ();

    ବାଛ (op) {
      ସ୍ଥିତି Op_Code_Blue_ମୂଲ୍ୟଗୁଡ଼ିକ:
      ସ୍ଥିତି OpCode_OtherBlues:
      ସ୍ଥିତି OpCode_FamilyBlues:
      ସ୍ଥିତି OpCode_FamilyOtherBlues:
      ସ୍ଥିତି OpCode_StemSnapH:
      ସ୍ଥିତି OpCode_StemSnapV:
      ସ୍ଥିତି OpCode_StdHW:
      ସ୍ଥିତି OpCode_StdVW:
      ସ୍ଥିତି OpCode_BlueScale:
      ସ୍ଥିତି OpCode_BlueShift:
      ସ୍ଥିତି OpCode_BlueFuzz:
      ସ୍ଥିତି OpCode_ForceBold:
      ସ୍ଥିତି OpCode_LanguageGroup:
      ସ୍ଥିତି OpCode_ExpansionFactor:
      ସ୍ଥିତି OpCode_initialRandomSeed:
      ସ୍ଥିତି OpCode_defaultWidthX:
      ସ୍ଥିତି OpCode_nominalWidthX:
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_Subrs:
	dictval.subrsOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	dict_opset_t::process_op (op, env);
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ସଂରଚନା cff1_private_dict_opset_subset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, ଅଙ୍କ_interp_env_t& env, cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_subset_t& dictval)
  {
    ବାଛ (op) {
      ସ୍ଥିତି Op_Code_Blue_ମୂଲ୍ୟଗୁଡ଼ିକ:
      ସ୍ଥିତି OpCode_OtherBlues:
      ସ୍ଥିତି OpCode_FamilyBlues:
      ସ୍ଥିତି OpCode_FamilyOtherBlues:
      ସ୍ଥିତି OpCode_StemSnapH:
      ସ୍ଥିତି OpCode_StemSnapV:
      ସ୍ଥିତି OpCode_StdHW:
      ସ୍ଥିତି OpCode_StdVW:
      ସ୍ଥିତି OpCode_BlueScale:
      ସ୍ଥିତି OpCode_BlueShift:
      ସ୍ଥିତି OpCode_BlueFuzz:
      ସ୍ଥିତି OpCode_ForceBold:
      ସ୍ଥିତି OpCode_LanguageGroup:
      ସ୍ଥିତି OpCode_ExpansionFactor:
      ସ୍ଥିତି OpCode_initialRandomSeed:
      ସ୍ଥିତି OpCode_defaultWidthX:
      ସ୍ଥିତି OpCode_nominalWidthX:
	env.clear_args ();
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_Subrs:
	dictval.subrsOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	dict_opset_t::process_op (op, env);
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    dictval.add_op (op, env.str_ref);
  }
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା dict_interpreter_t<cff1_ଶୀର୍ଷ_dict_opset_t, cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t, cff1_ଶୀର୍ଷ_dict_interp_env_t> cff1_ଶୀର୍ଷ_dict_interpreter_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t> cff1_font_dict_interpreter_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା CFF1Index CFF1NameIndex;
ପ୍ରକାରବ୍ୟାଖ୍ୟା CFF1Index CFF1_ଶୀର୍ଷ_Dict_Index;

ସଂରଚନା cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t
{
  cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t() { init (); }

  ଶୂନ୍ୟ init () { init ( &Null (cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t), CFF_UNDEF_SID ); }

  ଶୂନ୍ୟ init (ଅଚଳ cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t *base_,
	     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ base->get_ଗଣନା (); }

  ଅଚଳ op_str_t &operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ { ଫେରନ୍ତୁ (*base)[i]; }

  ଅଚଳ cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t    *base;
  table_info_t		   privateDictInfo;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		fontName;
};

ସଂରଚନା CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ଫେରନ୍ତୁ FDArray<HBUINT16>::serialize<cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t, cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

ନାମାକାଶ OT {

using ନାମାକାଶ CFF;

ସଂରଚନା cff1
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  ସଂରଚନା accelerator_templ_t
  {
    ସ୍ଥିର constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      ଯଦି (!face) ଫେରନ୍ତୁ;

      ଶୀର୍ଷ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      ଏହି->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (ଏହି->blob);
      sc.start_processing ();

      ଅଚଳ OT::cff1 *cff = ଏହି->blob->template as<OT::cff1> ();

      ଯଦି (cff == &Null (OT::cff1))
        ଯାଅ fail;

      nameIndex = &cff->nameIndex (cff);
      ଯଦି ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        ଯାଅ fail;
      hb_barrier ();

      ଶୀର୍ଷ_Dict_Index = &StructAtOffsetOrNull<CFF1_ଶୀର୍ଷ_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      ଯଦି (ଶୀର୍ଷ_Dict_Index == &Null (CFF1_ଶୀର୍ଷ_Dict_Index) || (ଶୀର୍ଷ_Dict_Index->ଗଣନା_2 == 0))
        ଯାଅ fail;
      hb_barrier ();

      { /* parse top dict */
	ଅଚଳ hb_ubytes_t ଶୀର୍ଷ_Dict_Str = (*ଶୀର୍ଷ_Dict_Index)[0];
	ଯଦି (unlikely (!ଶୀର୍ଷ_Dict_Str.sanitize (&sc)))   ଯାଅ fail;
	hb_barrier ();
	cff1_ଶୀର୍ଷ_dict_interp_env_t env (ଶୀର୍ଷ_Dict_Str);
	cff1_ଶୀର୍ଷ_dict_interpreter_t ଶୀର୍ଷ_interp (env);
	ଯଦି (unlikely (!ଶୀର୍ଷ_interp.interpret (ଶୀର୍ଷ_Dict_2)))   ଯାଅ fail;
      }

      ଯଦି (is_predef_charset ())
	charset = &Null (Charset);
      ଅନ୍ୟଥା
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, ଶୀର୍ଷ_Dict_2.CharsetOffset, sc, &ଅଙ୍କ_charset_entries);
	ଯଦି (unlikely (charset == &Null (Charset)))   ଯାଅ fail;
      }

      fd_ଗଣନା = 1;
      ଯଦି (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, ଶୀର୍ଷ_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, ଶୀର୍ଷ_Dict_2.FDSelectOffset, sc, fdArray->ଗଣନା_2);
	ଯଦି (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  ଯାଅ fail;

	fd_ଗଣନା = fdArray->ଗଣନା_2;
      }
      ଅନ୍ୟଥା
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      ଯଦି (is_CID ())
      {
	ଯଦି (unlikely (charset == &Null (Charset)))   ଯାଅ fail;
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, ଶୀର୍ଷ_Dict_2.EncodingOffset, sc);
	  ଯଦି (unlikely (encoding == &Null (Encoding)))   ଯାଅ fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (ଶୀର୍ଷ_Dict_Index, ଶୀର୍ଷ_Dict_Index->get_size (), sc);
      ଯଦି (stringIndex == &Null (CFF1StringIndex))
        ଯାଅ fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, ଶୀର୍ଷ_Dict_2.charStringsOffset, sc);
      ଯଦି (charStrings == &Null (CFF1CharStrings))
        ଯାଅ fail;

      ଅଙ୍କ_glyphs = charStrings->ଗଣନା_2;
      ଯଦି (ଅଙ୍କ_glyphs != sc.get_ଅଙ୍କ_glyphs ())
        ଯାଅ fail;

      ଯଦି (unlikely (!privateDicts.resize (fd_ଗଣନା)))
        ଯାଅ fail;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < fd_ଗଣନା; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      ଯଦି (is_CID ())
      {
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < fd_ଗଣନା; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  ଯଦି (unlikely (!fontDictStr.sanitize (&sc)))   ଯାଅ fail;
	  hb_barrier ();
	  cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t *font;
	  cff1_ଶୀର୍ଷ_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  ଯଦି (unlikely (fontDicts.in_error ()))   ଯାଅ fail;

	  font->init ();
	  ଯଦି (unlikely (!font_interp.interpret (*font)))   ଯାଅ fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  ଅଚଳ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  ଯଦି (unlikely (font->privateDictInfo.size &&
			privDictStr == (ଅଚଳ unsigned ଅକ୍ଷର *) &Null (UnsizedByteStr))) ଯାଅ fail;
	  ଅଙ୍କ_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  ଯଦି (unlikely (!priv_interp.interpret (*priv)))   ଯାଅ fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      ଅନ୍ୟଥା  /* non-CID */
      {
	cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t *font = &ଶୀର୍ଷ_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	ଅଚଳ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	ଯଦି (font->privateDictInfo.size &&
	    unlikely (privDictStr == (ଅଚଳ unsigned ଅକ୍ଷର *) &Null (UnsizedByteStr))) ଯାଅ fail;
	ଅଙ୍କ_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	ଯଦି (unlikely (!priv_interp.interpret (*priv)))   ଯାଅ fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      ଫେରନ୍ତୁ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    ଶୂନ୍ୟ _fini ()
    {
      sc.end_processing ();
      ଶୀର୍ଷ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () ଅଚଳ { ଫେରନ୍ତୁ blob; }

    ତାର୍କିକ is_valid () ଅଚଳ { ଫେରନ୍ତୁ blob; }
    ତାର୍କିକ   is_CID () ଅଚଳ { ଫେରନ୍ତୁ ଶୀର୍ଷ_Dict_2.is_CID (); }

    ତାର୍କିକ is_predef_charset () ଅଚଳ { ଫେରନ୍ତୁ ଶୀର୍ଷ_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା std_code_to_glyph (hb_codepoint_t code) ଅଚଳ
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      ଯଦି (unlikely (sid == CFF_UNDEF_SID))
	ଫେରନ୍ତୁ 0;

      ଯଦି (charset != &Null (Charset))
	ଫେରନ୍ତୁ charset->get_glyph (sid, ଅଙ୍କ_glyphs);
      ଅନ୍ୟଥା ଯଦି ((ଶୀର୍ଷ_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) ଫେରନ୍ତୁ sid;
      ଫେରନ୍ତୁ 0;
    }

    ତାର୍କିକ is_predef_encoding () ଅଚଳ { ଫେରନ୍ତୁ ଶୀର୍ଷ_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) ଅଚଳ
    {
      ଯଦି (encoding != &Null (Encoding))
	ଫେରନ୍ତୁ encoding->get_code (glyph);
      ଅନ୍ୟଥା
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	ଯଦି (sid == 0) ଫେରନ୍ତୁ 0;
	hb_codepoint_t code = 0;
	ବାଛ (ଶୀର୍ଷ_Dict_2.EncodingOffset)
	{
	ସ୍ଥିତି StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  ଭାଙ୍ଗ;
	ସ୍ଥିତି ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	  ଭାଙ୍ଗ;
	}
	ଫେରନ୍ତୁ code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () ଅଚଳ
    {
      ଯଦି (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (ଆକାର (glyph_to_sid_map_t));
	ଯଦି (unlikely (!mapping)) ଫେରନ୍ତୁ nullptr;
	mapping = ନୂତନ (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, ଅଙ୍କ_glyphs);
	ଫେରନ୍ତୁ mapping;
      }
      ଅନ୍ୟଥା
	ଫେରନ୍ତୁ nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) ଅଚଳ
    {
      ଯଦି (charset != &Null (Charset))
	ଫେରନ୍ତୁ charset->get_sid (glyph, ଅଙ୍କ_glyphs, cache);
      ଅନ୍ୟଥା
      {
	hb_codepoint_t sid = 0;
	ବାଛ (ଶୀର୍ଷ_Dict_2.CharsetOffset)
	{
	  ସ୍ଥିତି ISOAdobeCharset:
	    ଯଦି (glyph <= 228 /*zcaron*/) sid = glyph;
	    ଭାଙ୍ଗ;
	  ସ୍ଥିତି ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    ଭାଙ୍ଗ;
	  ସ୍ଥିତି ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    ଭାଙ୍ଗ;
	  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	    ଭାଙ୍ଗ;
	}
	ଫେରନ୍ତୁ sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) ଅଚଳ
    {
      ଯଦି (charset != &Null (Charset))
	ଫେରନ୍ତୁ charset->get_glyph (sid, ଅଙ୍କ_glyphs);
      ଅନ୍ୟଥା
      {
	hb_codepoint_t glyph = 0;
	ବାଛ (ଶୀର୍ଷ_Dict_2.CharsetOffset)
	{
	  ସ୍ଥିତି ISOAdobeCharset:
	    ଯଦି (sid <= 228 /*zcaron*/) glyph = sid;
	    ଭାଙ୍ଗ;
	  ସ୍ଥିତି ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    ଭାଙ୍ଗ;
	  ସ୍ଥିତି ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    ଭାଙ୍ଗ;
	  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	    ଭାଙ୍ଗ;
	}
	ଫେରନ୍ତୁ glyph;
      }
    }

    ସୁରକ୍ଷିତ:
    hb_sanitize_context_t   sc;

    ସର୍ବସାଧାରଣ:
    hb_blob_t               *blob = nullptr;
    ଅଚଳ Encoding	    *encoding = nullptr;
    ଅଚଳ Charset	    *charset = nullptr;
    ଅଚଳ CFF1NameIndex     *nameIndex = nullptr;
    ଅଚଳ CFF1_ଶୀର୍ଷ_Dict_Index  *ଶୀର୍ଷ_Dict_Index = nullptr;
    ଅଚଳ CFF1StringIndex   *stringIndex = nullptr;
    ଅଚଳ CFF1Subrs	    *globalSubrs = nullptr;
    ଅଚଳ CFF1CharStrings   *charStrings = nullptr;
    ଅଚଳ CFF1FDArray       *fdArray = nullptr;
    ଅଚଳ CFF1FDSelect      *fdSelect = nullptr;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	     fd_ଗଣନା = 0;

    cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t   ଶୀର୍ଷ_Dict_2;
    hb_vector_t<cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	     ଅଙ୍କ_glyphs = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	     ଅଙ୍କ_charset_entries = 0;
  };

  ସଂରଚନା accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      ଯଦି (!is_valid ()) ଫେରନ୍ତୁ;
      ଯଦି (is_CID ()) ଫେରନ୍ତୁ;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      ଯଦି (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    ତାର୍କିକ get_glyph_name (hb_codepoint_t glyph,
			 ଅକ୍ଷର *buf, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା buf_len) ଅଚଳ
    {
      ଯଦି (unlikely (glyph >= ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (unlikely (!is_valid ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (is_CID()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (unlikely (!buf_len)) ଫେରନ୍ତୁ ସତ୍ୟ;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      ଅଚଳ ଅକ୍ଷର *str;
      size_t str_len;
      ଯଦି (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      ଅନ୍ୟଥା
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (ଅଚଳ ଅକ୍ଷର *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      ଯଦି (!str_len) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ଅଚଳ ଅକ୍ଷର*)str, len);
      buf[len] = '\0';
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ get_glyph_from_name (ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
			      hb_codepoint_t *glyph) ଅଚଳ
    {
      ଯଦି (unlikely (!is_valid ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (is_CID()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (len < 0) len = strlen (name);
      ଯଦି (unlikely (!len)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      ଯଦି (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, ଆକାର (hb_sorted_vector_t<gname_t>));
	ଯଦି (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  ପାଇଁ (hb_codepoint_t gid = 0; gid < ଅଙ୍କ_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    ଯଦି (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    ଅନ୍ୟଥା
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((ଅଚଳ ଅକ୍ଷର*) ustr.arrayZ, ustr.length);
	    }
	    ଯଦି (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	ଯଦି (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  ଯଦି (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  ଯାଅ retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      ଅଚଳ gname_t *gname = names ? names->bsearch (key) : nullptr;
      ଯଦି (!gname) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      ଯଦି (!gid && gname->sid) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      *glyph = gid;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    HB_INTERNAL ତାର୍କିକ get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) ଅଚଳ;
    HB_INTERNAL ତାର୍କିକ get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ଅଚଳ;

    ବ୍ୟକ୍ତିଗତ:
    ସଂରଚନା gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *a_, ଅଚଳ ଶୂନ୍ୟ *b_)
      {
	ଅଚଳ gname_t *a = (ଅଚଳ gname_t *)a_;
	ଅଚଳ gname_t *b = (ଅଚଳ gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	ଯଦି (ret) ଫେରନ୍ତୁ ret;
	ଫେରନ୍ତୁ a->name.length - b->name.length;
      }

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ gname_t &a) ଅଚଳ { ଫେରନ୍ତୁ cmp (&a, ଏହି); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    ପ୍ରକାରବ୍ୟାଖ୍ୟା accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t> SUPER;
  };

  ସଂରଚନା accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ଯଦି (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ;
    HB_INTERNAL ତାର୍କିକ serialize (hb_serialize_context_t *c,
				ସଂରଚନା cff1_subset_plan &plan) ଅଚଳ;
    HB_INTERNAL ତାର୍କିକ get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) ଅଚଳ;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    ପ୍ରକାରବ୍ୟାଖ୍ୟା accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_subset_t> SUPER;
  };

  ସୁରକ୍ଷିତ:
  HB_INTERNAL ସ୍ଥିର hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL ସ୍ଥିର hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL ସ୍ଥିର hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL ସ୍ଥିର hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL ସ୍ଥିର hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL ସ୍ଥିର hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL ସ୍ଥିର hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  ସର୍ବସାଧାରଣ:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

ସଂରଚନା cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *builder) ଅଚଳ
  {
    ଯଦି (!is_valid ()) ଫେରନ୍ତୁ;
    ପାଇଁ (hb_codepoint_t gid = 0; gid < ଅଙ୍କ_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      ଯଦି (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
