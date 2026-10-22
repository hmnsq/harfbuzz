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

#ಅನ್ತರ್ಭಾವ "hb-ot-cff-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-depend-data.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-draw.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#ಅನ್ತರ್ಭಾವ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

ನಾಮಾಕಾಶ CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

ಗಣನಾ EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
ಗಣನಾ CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

ಪ್ರಕಾರನಿರ್ದೇಶ CFF1Index          CFF1CharStrings;
ಪ್ರಕಾರನಿರ್ದೇಶ Subrs<HBUINT16>    CFF1Subrs;

ಸಂರಚನಾ CFF1FDSelect : FDSelect {};

/* Encoding */
ಸಂರಚನಾ Encoding0 {
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ಧ್ರುವ
  {
    ಪ್ರತಿಜ್ಞಾ (glyph > 0);
    glyph--;
    ಯದಿ (glyph < nCodes ())
    {
      ಪ್ರತ್ಯಾಗಮ (hb_codepoint_t)codes[glyph];
    }
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { ಪ್ರತ್ಯಾಗಮ codes.len; }
  HBUINT8 nCodes () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

ಸಂರಚನಾ Encoding1_Range {
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  HBUINT8   first;
  HBUINT8   n_ಎಡ;

  DEFINE_SIZE_STATIC (2);
};

ಸಂರಚನಾ Encoding1 {
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ಧ್ರುವ
  {
    /* TODO: Add cache like get_sid. */
    ಪ್ರತಿಜ್ಞಾ (glyph > 0);
    glyph--;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < nRanges (); i++)
    {
      ಯದಿ (glyph <= ranges[i].n_ಎಡ)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	ಪ್ರತ್ಯಾಗಮ (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_ಎಡ + 1);
    }
    ಪ್ರತ್ಯಾಗಮ CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { ಪ್ರತ್ಯಾಗಮ ranges.len; }
  HBUINT8 nRanges () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

ಸಂರಚನಾ SuppEncoding {
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

ಸಂರಚನಾ CFF1SuppEncData {
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (supps.sanitize (c));
  }

  ಶೂನ್ಯ get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < nSups (); i++)
      ಯದಿ (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { ಪ್ರತ್ಯಾಗಮ supps.len; }
  HBUINT8 nSups () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

ಸಂರಚನಾ Encoding
{
  /* serialize a fullset Encoding */
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ Encoding &src)
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned ಪೂರ್ಣಾಙ್ಕ enc_ಎಣಿಕೆ,
		  ಧ್ರುವ hb_vector_t<code_pair_t>& code_ranges,
		  ಧ್ರುವ hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (ಏತತ್);
    Encoding *dest = c->extend_min (ಏತತ್);
    ಯದಿ (unlikely (!dest)) return_trace (ಅಸತ್ಯ);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    ವಿಕಲ್ಪ (format) {
    ಪಕ್ಷ 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_ಎಣಿಕೆ);
      ಯದಿ (unlikely (!fmt0)) return_trace (ಅಸತ್ಯ);
      fmt0->nCodes () = enc_ಎಣಿಕೆ;
      unsigned ಪೂರ್ಣಾಙ್ಕ glyph = 0;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ left = (ಪೂರ್ಣಾಙ್ಕ)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	ಯದಿ (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (ಅಸತ್ಯ);
      }
    }
    ಭಙ್ಗ;

    ಪಕ್ಷ 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      ಯದಿ (unlikely (!fmt1)) return_trace (ಅಸತ್ಯ);
      fmt1->nRanges () = code_ranges.length;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < code_ranges.length; i++)
      {
	ಯದಿ (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (ಅಸತ್ಯ);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_ಎಡ = code_ranges[i].glyph;
      }
    }
    ಭಙ್ಗ;

    }

    ಯದಿ (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      ಯದಿ (unlikely (!suppData)) return_trace (ಅಸತ್ಯ);
      suppData->nSups () = supp_codes.length;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (ಸತ್ಯ);
  }

  size_t get_size () ಧ್ರುವ
  {
    size_t size = min_size;
    ವಿಕಲ್ಪ (table_format ())
    {
    ಪಕ್ಷ 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); ಭಙ್ಗ;
    ಪಕ್ಷ 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); ಭಙ್ಗ;
    }
    ಯದಿ (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    ಪ್ರತ್ಯಾಗಮ size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (table_format ())
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_code (glyph);
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_code (glyph);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  uint8_t table_format () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ format & 0x7F; }
  ತಾರ್ಕಿಕ  has_supplement () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ format & 0x80; }

  ಶೂನ್ಯ get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ಧ್ರುವ
  {
    codes.clear ();
    ಯದಿ (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ವಿಕಲ್ಪ (table_format ())
    {
    ಪಕ್ಷ 0: hb_barrier (); ಯದಿ (unlikely (!u.format0.sanitize (c))) { return_trace (ಅಸತ್ಯ); } ಭಙ್ಗ;
    ಪಕ್ಷ 1: hb_barrier (); ಯದಿ (unlikely (!u.format1.sanitize (c))) { return_trace (ಅಸತ್ಯ); } ಭಙ್ಗ;
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  ರಕ್ಷಿತ:
  ಧ್ರುವ CFF1SuppEncData &suppEncData () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (table_format ())
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ Null (CFF1SuppEncData);
    }
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT8	format;
  ಸಙ್ಘ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
ಸಂರಚನಾ Charset0
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs, unsigned *ಅಂಕೆ_charset_entries) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (ಅಂಕೆ_charset_entries) *ಅಂಕೆ_charset_entries = ಅಂಕೆ_glyphs;
    return_trace (sids.sanitize (c, ಅಂಕೆ_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಯದಿ (unlikely (glyph >= ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ 0;
    ಯದಿ (unlikely (glyph == 0))
      ಪ್ರತ್ಯಾಗಮ 0;
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ sids[glyph - 1];
  }

  ಶೂನ್ಯ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಯದಿ (!mapping->resize_dirty  (ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ (hb_codepoint_t gid = 1; gid < ಅಂಕೆ_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಯದಿ (sid == 0)
      ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ glyph = 1; glyph < ಅಂಕೆ_glyphs; glyph++)
    {
      ಯದಿ (sids[glyph-1] == sid)
	ಪ್ರತ್ಯಾಗಮ glyph;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  ಸ್ಥಿರ size_t get_size (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs)
  {
    ಪ್ರತಿಜ್ಞಾ (ಅಂಕೆ_glyphs > 0);
    ಪ್ರತ್ಯಾಗಮ UnsizedArrayOf<HBUINT16>::get_size (ಅಂಕೆ_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
ಸಂರಚನಾ Charset_Range {
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  HBUINT16  first;
  TYPE      n_ಎಡ;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
ಸಂರಚನಾ Charset1_2 {
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs, unsigned *ಅಂಕೆ_charset_entries) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಅಂಕೆ_glyphs--;
    unsigned i;
    ಪ್ರತಿ (i = 0; ಅಂಕೆ_glyphs > 0; i++)
    {
      ಯದಿ (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (ಅಂಕೆ_glyphs >= ranges[i].n_ಎಡ + 1))))
	return_trace (ಅಸತ್ಯ);
      ಅಂಕೆ_glyphs -= (ranges[i].n_ಎಡ + 1);
    }
    ಯದಿ (ಅಂಕೆ_charset_entries)
      *ಅಂಕೆ_charset_entries = i;
    return_trace (ಸತ್ಯ);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned ಅಂಕೆ_glyphs,
			  code_pair_t *cache = nullptr) ಧ್ರುವ
  {
    ಯದಿ (unlikely (glyph >= ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    ಯದಿ (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (unlikely (glyph == 0)) ಪ್ರತ್ಯಾಗಮ 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    ಪ್ರತಿ (;; i++)
    {
      unsigned ಎಣಿಕೆ = ranges[i].n_ಎಡ;
      ಯದಿ (glyph <= ಎಣಿಕೆ)
      {
        ಯದಿ (cache)
	  *cache = {i, start_glyph};
	ಪ್ರತ್ಯಾಗಮ ranges[i].first + glyph;
      }
      ಎಣಿಕೆ++;
      start_glyph += ಎಣಿಕೆ;
      glyph -= ಎಣಿಕೆ;
    }

    ಪ್ರತ್ಯಾಗಮ 0;
  }

  ಶೂನ್ಯ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಯದಿ (!mapping->resize_dirty  (ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ;

    hb_codepoint_t gid = 1;
    ಯದಿ (gid >= ಅಂಕೆ_glyphs)
      ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned ಎಣಿಕೆ = ranges[i].n_ಎಡ + 1;
      unsigned last = gid + ಎಣಿಕೆ;
      ಪ್ರತಿ (unsigned j = 0; j < ಎಣಿಕೆ; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      ಯದಿ (gid >= ಅಂಕೆ_glyphs)
        ಭಙ್ಗ;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಯದಿ (sid == 0) ಪ್ರತ್ಯಾಗಮ 0;
    hb_codepoint_t  glyph = 1;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;; i++)
    {
      ಯದಿ (glyph >= ಅಂಕೆ_glyphs)
	ಪ್ರತ್ಯಾಗಮ 0;
      ಯದಿ ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_ಎಡ))
	ಪ್ರತ್ಯಾಗಮ glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_ಎಡ + 1);
    }

    ಪ್ರತ್ಯಾಗಮ 0;
  }

  size_t get_size (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಪೂರ್ಣಾಙ್ಕ glyph = (ಪೂರ್ಣಾಙ್ಕ) ಅಂಕೆ_glyphs;
    unsigned ಅಂಕೆ_ranges = 0;

    ಪ್ರತಿಜ್ಞಾ (glyph > 0);
    glyph--;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_ಎಡ + 1);
      ಅಂಕೆ_ranges++;
    }

    ಪ್ರತ್ಯಾಗಮ get_size_for_ranges (ಅಂಕೆ_ranges);
  }

  ಸ್ಥಿರ size_t get_size_for_ranges (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ranges)
  {
    ಪ್ರತ್ಯಾಗಮ UnsizedArrayOf<Charset_Range<TYPE> >::get_size (ಅಂಕೆ_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

ಪ್ರಕಾರನಿರ್ದೇಶ Charset1_2<HBUINT8>     Charset1;
ಪ್ರಕಾರನಿರ್ದೇಶ Charset1_2<HBUINT16>    Charset2;
ಪ್ರಕಾರನಿರ್ದೇಶ Charset_Range<HBUINT8>  Charset1_Range;
ಪ್ರಕಾರನಿರ್ದೇಶ Charset_Range<HBUINT16> Charset2_Range;

ಸಂರಚನಾ Charset
{
  /* serialize a fullset Charset */
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ Charset &src, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed ((ಧ್ರುವ ವರ್ಣ *) &src, src.get_size (ಅಂಕೆ_glyphs)));
  }

  /* serialize a subset Charset */
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
		  ಧ್ರುವ hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (ಏತತ್);
    Charset *dest = c->extend_min (ಏತತ್);
    ಯದಿ (unlikely (!dest)) return_trace (ಅಸತ್ಯ);
    dest->format = format;
    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (ಅಂಕೆ_glyphs), ಅಸತ್ಯ);
      ಯದಿ (unlikely (!fmt0)) return_trace (ಅಸತ್ಯ);
      unsigned ಪೂರ್ಣಾಙ್ಕ glyph = 0;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ left = (ಪೂರ್ಣಾಙ್ಕ)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    ಭಙ್ಗ;

    ಪಕ್ಷ 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), ಅಸತ್ಯ);
      ಯದಿ (unlikely (!fmt1)) return_trace (ಅಸತ್ಯ);
      hb_codepoint_t all_glyphs = 0;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_ಎಡ = _.glyph;
      }
      ಯದಿ (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (ಅಸತ್ಯ);
    }
    ಭಙ್ಗ;

    ಪಕ್ಷ 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), ಅಸತ್ಯ);
      ಯದಿ (unlikely (!fmt2)) return_trace (ಅಸತ್ಯ);
      hb_codepoint_t all_glyphs = 0;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_ಎಡ = _.glyph;
      }
      ಯದಿ (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (ಅಸತ್ಯ);
    }
    ಭಙ್ಗ;

    }
    return_trace (ಸತ್ಯ);
  }

  size_t get_size (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (min_size, u.format0.get_size (ಅಂಕೆ_glyphs));
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (min_size, u.format1.get_size (ಅಂಕೆ_glyphs));
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (min_size, u.format2.get_size (ಅಂಕೆ_glyphs));
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
			  code_pair_t *cache = nullptr) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_sid (glyph, ಅಂಕೆ_glyphs);
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_sid (glyph, ಅಂಕೆ_glyphs, cache);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_sid (glyph, ಅಂಕೆ_glyphs, cache);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ಶೂನ್ಯ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_glyph (sid, ಅಂಕೆ_glyphs);
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_glyph (sid, ಅಂಕೆ_glyphs);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_glyph (sid, ಅಂಕೆ_glyphs);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned *ಅಂಕೆ_charset_entries) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_ಅಂಕೆ_glyphs (), ಅಂಕೆ_charset_entries));
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_ಅಂಕೆ_glyphs (), ಅಂಕೆ_charset_entries));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_ಅಂಕೆ_glyphs (), ಅಂಕೆ_charset_entries));
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  HBUINT8       format;
  ಸಙ್ಘ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

ಸಂರಚನಾ CFF1StringIndex : CFF1Index
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ CFF1StringIndex &strings,
		  ಧ್ರುವ hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely ((strings.ಎಣಿಕೆ == 0) || (sidmap.length == 0)))
    {
      ಯದಿ (unlikely (!c->extend_min (ಏತತ್->ಎಣಿಕೆ)))
	return_trace (ಅಸತ್ಯ);
      ಎಣಿಕೆ = 0;
      return_trace (ಸತ್ಯ);
    }

    ಯದಿ (unlikely (sidmap.in_error ())) return_trace (ಅಸತ್ಯ);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    ಯದಿ (unlikely (bytesArray.in_error ())) return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

ಸಂರಚನಾ cff1_ತುದಿ_dict_interp_env_t : ಅಂಕೆ_interp_env_t
{
  cff1_ತುದಿ_dict_interp_env_t ()
    : ಅಂಕೆ_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_ತುದಿ_dict_interp_env_t (ಧ್ರುವ hb_ubytes_t &bytes)
    : ಅಂಕೆ_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned ಪೂರ್ಣಾಙ್ಕ prev_offset;
  unsigned ಪೂರ್ಣಾಙ್ಕ last_offset;
};

ಸಂರಚನಾ name_dict_ಮೌಲ್ಯಗಳು_t
{
  ಗಣನಾ name_dict_val_index_t
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

      Val_ಎಣಿಕೆ
  };

  ಶೂನ್ಯ init ()
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < Val_ಎಣಿಕೆ; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ& operator[] (unsigned ಪೂರ್ಣಾಙ್ಕ i)
  { ಪ್ರತಿಜ್ಞಾ (i < Val_ಎಣಿಕೆ); ಪ್ರತ್ಯಾಗಮ values[i]; }

  unsigned ಪೂರ್ಣಾಙ್ಕ operator[] (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತಿಜ್ಞಾ (i < Val_ಎಣಿಕೆ); ಪ್ರತ್ಯಾಗಮ values[i]; }

  ಸ್ಥಿರ ಗಣನಾ name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    ವಿಕಲ್ಪ (op) {
      ಸ್ವಭಾವ: // can't happen - just make some compiler happy
      ಪಕ್ಷ OpCode_version:
	ಪ್ರತ್ಯಾಗಮ version;
      ಪಕ್ಷ OpCode_Notice:
	ಪ್ರತ್ಯಾಗಮ notice;
      ಪಕ್ಷ OpCode_Copyright:
	ಪ್ರತ್ಯಾಗಮ copyright;
      ಪಕ್ಷ OpCode_FullName:
	ಪ್ರತ್ಯಾಗಮ fullName;
      ಪಕ್ಷ OpCode_FamilyName:
	ಪ್ರತ್ಯಾಗಮ familyName;
      ಪಕ್ಷ OpCode_Weight:
	ಪ್ರತ್ಯಾಗಮ weight;
      ಪಕ್ಷ OpCode_PostScript:
	ಪ್ರತ್ಯಾಗಮ postscript;
      ಪಕ್ಷ OpCode_FontName:
	ಪ್ರತ್ಯಾಗಮ fontName;
      ಪಕ್ಷ OpCode_BaseFontName:
	ಪ್ರತ್ಯಾಗಮ baseFontName;
    }
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ  values[Val_ಎಣಿಕೆ];
};

ಸಂರಚನಾ cff1_ತುದಿ_dict_val_t : op_str_t
{
  unsigned ಪೂರ್ಣಾಙ್ಕ  last_arg_offset;
};

ಸಂರಚನಾ cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t : ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t<cff1_ತುದಿ_dict_val_t>
{
  ಶೂನ್ಯ init ()
  {
    ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t<cff1_ತುದಿ_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_ಎಣಿಕೆ = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  ಶೂನ್ಯ fini () { ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t<cff1_ತುದಿ_dict_val_t>::fini (); }

  ತಾರ್ಕಿಕ is_CID () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ nameSIDs[name_dict_ಮೌಲ್ಯಗಳು_t::registry] != CFF_UNDEF_SID; }

  name_dict_ಮೌಲ್ಯಗಳು_t  nameSIDs;
  unsigned ಪೂರ್ಣಾಙ್ಕ    ros_supplement_offset;
  unsigned ಪೂರ್ಣಾಙ್ಕ    ros_supplement;
  unsigned ಪೂರ್ಣಾಙ್ಕ    cid_ಎಣಿಕೆ;

  ಪೂರ್ಣಾಙ್ಕ             EncodingOffset;
  ಪೂರ್ಣಾಙ್ಕ             CharsetOffset;
  ಪೂರ್ಣಾಙ್ಕ             FDSelectOffset;
  table_info_t    privateDictInfo;
};

ಸಂರಚನಾ cff1_ತುದಿ_dict_opset_t : ತುದಿ_dict_opset_t<cff1_ತುದಿ_dict_val_t>
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff1_ತುದಿ_dict_interp_env_t& env, cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t& dictval)
  {
    cff1_ತುದಿ_dict_val_t  val {};

    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_version:
      ಪಕ್ಷ OpCode_Notice:
      ಪಕ್ಷ OpCode_Copyright:
      ಪಕ್ಷ OpCode_FullName:
      ಪಕ್ಷ OpCode_FontName:
      ಪಕ್ಷ OpCode_FamilyName:
      ಪಕ್ಷ OpCode_Weight:
      ಪಕ್ಷ OpCode_PostScript:
      ಪಕ್ಷ OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_ಮೌಲ್ಯಗಳು_t::name_op_to_index (op)] = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_isFixedPitch:
      ಪಕ್ಷ OpCode_ItalicAngle:
      ಪಕ್ಷ OpCode_UnderlinePosition:
      ಪಕ್ಷ OpCode_UnderlineThickness:
      ಪಕ್ಷ OpCode_PaintType:
      ಪಕ್ಷ OpCode_CharstringType:
      ಪಕ್ಷ OpCode_UniqueID:
      ಪಕ್ಷ OpCode_StrokeWidth:
      ಪಕ್ಷ OpCode_SyntheticBase:
      ಪಕ್ಷ OpCode_CIDFontVersion:
      ಪಕ್ಷ OpCode_CIDFontRevision:
      ಪಕ್ಷ OpCode_CIDFontType:
      ಪಕ್ಷ OpCode_UIDBase:
      ಪಕ್ಷ OpCode_FontBBox:
      ಪಕ್ಷ OpCode_XUID:
      ಪಕ್ಷ OpCode_BaseFontBlend:
	env.clear_args ();
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_CIDCount:
	dictval.cid_ಎಣಿಕೆ = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_ROS:
	dictval.ros_supplement = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	dictval.nameSIDs[name_dict_ಮೌಲ್ಯಗಳು_t::ordering] = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	dictval.nameSIDs[name_dict_ಮೌಲ್ಯಗಳು_t::registry] = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	env.clear_args ();
	ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;
	ಪ್ರತಿಜ್ಞಾ (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_Encoding:
	dictval.EncodingOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಯದಿ (unlikely (dictval.EncodingOffset == 0)) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_charset:
	dictval.CharsetOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಯದಿ (unlikely (dictval.CharsetOffset == 0)) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	dictval.privateDictInfo.size = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	env.last_offset = env.str_ref.get_offset ();
	ತುದಿ_dict_opset_t<cff1_ತುದಿ_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ಸಂರಚನಾ cff1_font_dict_ಮೌಲ್ಯಗಳು_t : dict_ಮೌಲ್ಯಗಳು_t<op_str_t>
{
  ಶೂನ್ಯ init ()
  {
    dict_ಮೌಲ್ಯಗಳು_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  ಶೂನ್ಯ fini () { dict_ಮೌಲ್ಯಗಳು_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned ಪೂರ್ಣಾಙ್ಕ    fontName;
};

ಸಂರಚನಾ cff1_font_dict_opset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, ಅಂಕೆ_interp_env_t& env, cff1_font_dict_ಮೌಲ್ಯಗಳು_t& dictval)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_FontName:
	dictval.fontName = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_FontMatrix:
      ಪಕ್ಷ OpCode_PaintType:
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	dictval.privateDictInfo.size = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	dict_opset_t::process_op (op, env);
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
ಸಂರಚನಾ cff1_private_dict_ಮೌಲ್ಯಗಳು_base_t : dict_ಮೌಲ್ಯಗಳು_t<VAL>
{
  ಶೂನ್ಯ init ()
  {
    dict_ಮೌಲ್ಯಗಳು_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  ಶೂನ್ಯ fini () { dict_ಮೌಲ್ಯಗಳು_t<VAL>::fini (); }

  ಪೂರ್ಣಾಙ್ಕ                 subrsOffset;
  ಧ್ರುವ CFF1Subrs    *localSubrs;
};

ಪ್ರಕಾರನಿರ್ದೇಶ cff1_private_dict_ಮೌಲ್ಯಗಳು_base_t<op_str_t> cff1_private_dict_ಮೌಲ್ಯಗಳು_subset_t;
ಪ್ರಕಾರನಿರ್ದೇಶ cff1_private_dict_ಮೌಲ್ಯಗಳು_base_t<ಅಂಕೆ_dict_val_t> cff1_private_dict_ಮೌಲ್ಯಗಳು_t;

ಸಂರಚನಾ cff1_private_dict_opset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, ಅಂಕೆ_interp_env_t& env, cff1_private_dict_ಮೌಲ್ಯಗಳು_t& dictval)
  {
    ಅಂಕೆ_dict_val_t val;
    val.init ();

    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ Op_Code_Blue_ಮೌಲ್ಯಗಳು:
      ಪಕ್ಷ OpCode_OtherBlues:
      ಪಕ್ಷ OpCode_FamilyBlues:
      ಪಕ್ಷ OpCode_FamilyOtherBlues:
      ಪಕ್ಷ OpCode_StemSnapH:
      ಪಕ್ಷ OpCode_StemSnapV:
      ಪಕ್ಷ OpCode_StdHW:
      ಪಕ್ಷ OpCode_StdVW:
      ಪಕ್ಷ OpCode_BlueScale:
      ಪಕ್ಷ OpCode_BlueShift:
      ಪಕ್ಷ OpCode_BlueFuzz:
      ಪಕ್ಷ OpCode_ForceBold:
      ಪಕ್ಷ OpCode_LanguageGroup:
      ಪಕ್ಷ OpCode_ExpansionFactor:
      ಪಕ್ಷ OpCode_initialRandomSeed:
      ಪಕ್ಷ OpCode_defaultWidthX:
      ಪಕ್ಷ OpCode_nominalWidthX:
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_Subrs:
	dictval.subrsOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	dict_opset_t::process_op (op, env);
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ಸಂರಚನಾ cff1_private_dict_opset_subset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, ಅಂಕೆ_interp_env_t& env, cff1_private_dict_ಮೌಲ್ಯಗಳು_subset_t& dictval)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ Op_Code_Blue_ಮೌಲ್ಯಗಳು:
      ಪಕ್ಷ OpCode_OtherBlues:
      ಪಕ್ಷ OpCode_FamilyBlues:
      ಪಕ್ಷ OpCode_FamilyOtherBlues:
      ಪಕ್ಷ OpCode_StemSnapH:
      ಪಕ್ಷ OpCode_StemSnapV:
      ಪಕ್ಷ OpCode_StdHW:
      ಪಕ್ಷ OpCode_StdVW:
      ಪಕ್ಷ OpCode_BlueScale:
      ಪಕ್ಷ OpCode_BlueShift:
      ಪಕ್ಷ OpCode_BlueFuzz:
      ಪಕ್ಷ OpCode_ForceBold:
      ಪಕ್ಷ OpCode_LanguageGroup:
      ಪಕ್ಷ OpCode_ExpansionFactor:
      ಪಕ್ಷ OpCode_initialRandomSeed:
      ಪಕ್ಷ OpCode_defaultWidthX:
      ಪಕ್ಷ OpCode_nominalWidthX:
	env.clear_args ();
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_Subrs:
	dictval.subrsOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	dict_opset_t::process_op (op, env);
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    dictval.add_op (op, env.str_ref);
  }
};

ಪ್ರಕಾರನಿರ್ದೇಶ dict_interpreter_t<cff1_ತುದಿ_dict_opset_t, cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t, cff1_ತುದಿ_dict_interp_env_t> cff1_ತುದಿ_dict_interpreter_t;
ಪ್ರಕಾರನಿರ್ದೇಶ dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_ಮೌಲ್ಯಗಳು_t> cff1_font_dict_interpreter_t;

ಪ್ರಕಾರನಿರ್ದೇಶ CFF1Index CFF1NameIndex;
ಪ್ರಕಾರನಿರ್ದೇಶ CFF1Index CFF1_ತುದಿ_Dict_Index;

ಸಂರಚನಾ cff1_font_dict_ಮೌಲ್ಯಗಳು_mod_t
{
  cff1_font_dict_ಮೌಲ್ಯಗಳು_mod_t() { init (); }

  ಶೂನ್ಯ init () { init ( &Null (cff1_font_dict_ಮೌಲ್ಯಗಳು_t), CFF_UNDEF_SID ); }

  ಶೂನ್ಯ init (ಧ್ರುವ cff1_font_dict_ಮೌಲ್ಯಗಳು_t *base_,
	     unsigned ಪೂರ್ಣಾಙ್ಕ fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ base->get_ಎಣಿಕೆ (); }

  ಧ್ರುವ op_str_t &operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*base)[i]; }

  ಧ್ರುವ cff1_font_dict_ಮೌಲ್ಯಗಳು_t    *base;
  table_info_t		   privateDictInfo;
  unsigned ಪೂರ್ಣಾಙ್ಕ		fontName;
};

ಸಂರಚನಾ CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ಪ್ರತ್ಯಾಗಮ FDArray<HBUINT16>::serialize<cff1_font_dict_ಮೌಲ್ಯಗಳು_mod_t, cff1_font_dict_ಮೌಲ್ಯಗಳು_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

ನಾಮಾಕಾಶ OT {

using ನಾಮಾಕಾಶ CFF;

ಸಂರಚನಾ cff1
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  ಸಂರಚನಾ accelerator_templ_t
  {
    ಸ್ಥಿರ constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      ಯದಿ (!face) ಪ್ರತ್ಯಾಗಮ;

      ತುದಿ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      ಏತತ್->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (ಏತತ್->blob);
      sc.start_processing ();

      ಧ್ರುವ OT::cff1 *cff = ಏತತ್->blob->template as<OT::cff1> ();

      ಯದಿ (cff == &Null (OT::cff1))
        ಗಚ್ಛ fail;

      nameIndex = &cff->nameIndex (cff);
      ಯದಿ ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        ಗಚ್ಛ fail;
      hb_barrier ();

      ತುದಿ_Dict_Index = &StructAtOffsetOrNull<CFF1_ತುದಿ_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      ಯದಿ (ತುದಿ_Dict_Index == &Null (CFF1_ತುದಿ_Dict_Index) || (ತುದಿ_Dict_Index->ಎಣಿಕೆ == 0))
        ಗಚ್ಛ fail;
      hb_barrier ();

      { /* parse top dict */
	ಧ್ರುವ hb_ubytes_t ತುದಿ_Dict_Str = (*ತುದಿ_Dict_Index)[0];
	ಯದಿ (unlikely (!ತುದಿ_Dict_Str.sanitize (&sc)))   ಗಚ್ಛ fail;
	hb_barrier ();
	cff1_ತುದಿ_dict_interp_env_t env (ತುದಿ_Dict_Str);
	cff1_ತುದಿ_dict_interpreter_t ತುದಿ_interp (env);
	ಯದಿ (unlikely (!ತುದಿ_interp.interpret (ತುದಿ_Dict_2)))   ಗಚ್ಛ fail;
      }

      ಯದಿ (is_predef_charset ())
	charset = &Null (Charset);
      ಅನ್ಯಥಾ
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, ತುದಿ_Dict_2.CharsetOffset, sc, &ಅಂಕೆ_charset_entries);
	ಯದಿ (unlikely (charset == &Null (Charset)))   ಗಚ್ಛ fail;
      }

      fd_ಎಣಿಕೆ = 1;
      ಯದಿ (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, ತುದಿ_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, ತುದಿ_Dict_2.FDSelectOffset, sc, fdArray->ಎಣಿಕೆ);
	ಯದಿ (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  ಗಚ್ಛ fail;

	fd_ಎಣಿಕೆ = fdArray->ಎಣಿಕೆ;
      }
      ಅನ್ಯಥಾ
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      ಯದಿ (is_CID ())
      {
	ಯದಿ (unlikely (charset == &Null (Charset)))   ಗಚ್ಛ fail;
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, ತುದಿ_Dict_2.EncodingOffset, sc);
	  ಯದಿ (unlikely (encoding == &Null (Encoding)))   ಗಚ್ಛ fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (ತುದಿ_Dict_Index, ತುದಿ_Dict_Index->get_size (), sc);
      ಯದಿ (stringIndex == &Null (CFF1StringIndex))
        ಗಚ್ಛ fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, ತುದಿ_Dict_2.charStringsOffset, sc);
      ಯದಿ (charStrings == &Null (CFF1CharStrings))
        ಗಚ್ಛ fail;

      ಅಂಕೆ_glyphs = charStrings->ಎಣಿಕೆ;
      ಯದಿ (ಅಂಕೆ_glyphs != sc.get_ಅಂಕೆ_glyphs ())
        ಗಚ್ಛ fail;

      ಯದಿ (unlikely (!privateDicts.resize (fd_ಎಣಿಕೆ)))
        ಗಚ್ಛ fail;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < fd_ಎಣಿಕೆ; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      ಯದಿ (is_CID ())
      {
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < fd_ಎಣಿಕೆ; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  ಯದಿ (unlikely (!fontDictStr.sanitize (&sc)))   ಗಚ್ಛ fail;
	  hb_barrier ();
	  cff1_font_dict_ಮೌಲ್ಯಗಳು_t *font;
	  cff1_ತುದಿ_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  ಯದಿ (unlikely (fontDicts.in_error ()))   ಗಚ್ಛ fail;

	  font->init ();
	  ಯದಿ (unlikely (!font_interp.interpret (*font)))   ಗಚ್ಛ fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  ಧ್ರುವ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  ಯದಿ (unlikely (font->privateDictInfo.size &&
			privDictStr == (ಧ್ರುವ unsigned ವರ್ಣ *) &Null (UnsizedByteStr))) ಗಚ್ಛ fail;
	  ಅಂಕೆ_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  ಯದಿ (unlikely (!priv_interp.interpret (*priv)))   ಗಚ್ಛ fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      ಅನ್ಯಥಾ  /* non-CID */
      {
	cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t *font = &ತುದಿ_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	ಧ್ರುವ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	ಯದಿ (font->privateDictInfo.size &&
	    unlikely (privDictStr == (ಧ್ರುವ unsigned ವರ್ಣ *) &Null (UnsizedByteStr))) ಗಚ್ಛ fail;
	ಅಂಕೆ_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	ಯದಿ (unlikely (!priv_interp.interpret (*priv)))   ಗಚ್ಛ fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      ಪ್ರತ್ಯಾಗಮ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    ಶೂನ್ಯ _fini ()
    {
      sc.end_processing ();
      ತುದಿ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ blob; }

    ತಾರ್ಕಿಕ is_valid () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ blob; }
    ತಾರ್ಕಿಕ   is_CID () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತುದಿ_Dict_2.is_CID (); }

    ತಾರ್ಕಿಕ is_predef_charset () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತುದಿ_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned ಪೂರ್ಣಾಙ್ಕ std_code_to_glyph (hb_codepoint_t code) ಧ್ರುವ
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      ಯದಿ (unlikely (sid == CFF_UNDEF_SID))
	ಪ್ರತ್ಯಾಗಮ 0;

      ಯದಿ (charset != &Null (Charset))
	ಪ್ರತ್ಯಾಗಮ charset->get_glyph (sid, ಅಂಕೆ_glyphs);
      ಅನ್ಯಥಾ ಯದಿ ((ತುದಿ_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) ಪ್ರತ್ಯಾಗಮ sid;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

    ತಾರ್ಕಿಕ is_predef_encoding () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತುದಿ_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) ಧ್ರುವ
    {
      ಯದಿ (encoding != &Null (Encoding))
	ಪ್ರತ್ಯಾಗಮ encoding->get_code (glyph);
      ಅನ್ಯಥಾ
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	ಯದಿ (sid == 0) ಪ್ರತ್ಯಾಗಮ 0;
	hb_codepoint_t code = 0;
	ವಿಕಲ್ಪ (ತುದಿ_Dict_2.EncodingOffset)
	{
	ಪಕ್ಷ StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  ಭಙ್ಗ;
	ಪಕ್ಷ ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  ಭಙ್ಗ;
	ಸ್ವಭಾವ:
	  ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () ಧ್ರುವ
    {
      ಯದಿ (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (ಪರಿಮಾಣ (glyph_to_sid_map_t));
	ಯದಿ (unlikely (!mapping)) ಪ್ರತ್ಯಾಗಮ nullptr;
	mapping = ನೂತನ (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, ಅಂಕೆ_glyphs);
	ಪ್ರತ್ಯಾಗಮ mapping;
      }
      ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) ಧ್ರುವ
    {
      ಯದಿ (charset != &Null (Charset))
	ಪ್ರತ್ಯಾಗಮ charset->get_sid (glyph, ಅಂಕೆ_glyphs, cache);
      ಅನ್ಯಥಾ
      {
	hb_codepoint_t sid = 0;
	ವಿಕಲ್ಪ (ತುದಿ_Dict_2.CharsetOffset)
	{
	  ಪಕ್ಷ ISOAdobeCharset:
	    ಯದಿ (glyph <= 228 /*zcaron*/) sid = glyph;
	    ಭಙ್ಗ;
	  ಪಕ್ಷ ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    ಭಙ್ಗ;
	  ಪಕ್ಷ ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    ಭಙ್ಗ;
	  ಸ್ವಭಾವ:
	    ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) ಧ್ರುವ
    {
      ಯದಿ (charset != &Null (Charset))
	ಪ್ರತ್ಯಾಗಮ charset->get_glyph (sid, ಅಂಕೆ_glyphs);
      ಅನ್ಯಥಾ
      {
	hb_codepoint_t glyph = 0;
	ವಿಕಲ್ಪ (ತುದಿ_Dict_2.CharsetOffset)
	{
	  ಪಕ್ಷ ISOAdobeCharset:
	    ಯದಿ (sid <= 228 /*zcaron*/) glyph = sid;
	    ಭಙ್ಗ;
	  ಪಕ್ಷ ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    ಭಙ್ಗ;
	  ಪಕ್ಷ ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    ಭಙ್ಗ;
	  ಸ್ವಭಾವ:
	    ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ glyph;
      }
    }

    ರಕ್ಷಿತ:
    hb_sanitize_context_t   sc;

    ಸಾರ್ವಜನಿಕ:
    hb_blob_t               *blob = nullptr;
    ಧ್ರುವ Encoding	    *encoding = nullptr;
    ಧ್ರುವ Charset	    *charset = nullptr;
    ಧ್ರುವ CFF1NameIndex     *nameIndex = nullptr;
    ಧ್ರುವ CFF1_ತುದಿ_Dict_Index  *ತುದಿ_Dict_Index = nullptr;
    ಧ್ರುವ CFF1StringIndex   *stringIndex = nullptr;
    ಧ್ರುವ CFF1Subrs	    *globalSubrs = nullptr;
    ಧ್ರುವ CFF1CharStrings   *charStrings = nullptr;
    ಧ್ರುವ CFF1FDArray       *fdArray = nullptr;
    ಧ್ರುವ CFF1FDSelect      *fdSelect = nullptr;
    unsigned ಪೂರ್ಣಾಙ್ಕ	     fd_ಎಣಿಕೆ = 0;

    cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t   ತುದಿ_Dict_2;
    hb_vector_t<cff1_font_dict_ಮೌಲ್ಯಗಳು_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned ಪೂರ್ಣಾಙ್ಕ	     ಅಂಕೆ_glyphs = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ	     ಅಂಕೆ_charset_entries = 0;
  };

  ಸಂರಚನಾ accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_ಮೌಲ್ಯಗಳು_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      ಯದಿ (!is_valid ()) ಪ್ರತ್ಯಾಗಮ;
      ಯದಿ (is_CID ()) ಪ್ರತ್ಯಾಗಮ;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      ಯದಿ (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    ತಾರ್ಕಿಕ get_glyph_name (hb_codepoint_t glyph,
			 ವರ್ಣ *buf, unsigned ಪೂರ್ಣಾಙ್ಕ buf_len) ಧ್ರುವ
    {
      ಯದಿ (unlikely (glyph >= ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (unlikely (!is_valid ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (is_CID()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (unlikely (!buf_len)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      ಧ್ರುವ ವರ್ಣ *str;
      size_t str_len;
      ಯದಿ (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      ಅನ್ಯಥಾ
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (ಧ್ರುವ ವರ್ಣ *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      ಯದಿ (!str_len) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      unsigned ಪೂರ್ಣಾಙ್ಕ len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ಧ್ರುವ ವರ್ಣ*)str, len);
      buf[len] = '\0';
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ get_glyph_from_name (ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ len,
			      hb_codepoint_t *glyph) ಧ್ರುವ
    {
      ಯದಿ (unlikely (!is_valid ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (is_CID()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (len < 0) len = strlen (name);
      ಯದಿ (unlikely (!len)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      ಯದಿ (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, ಪರಿಮಾಣ (hb_sorted_vector_t<gname_t>));
	ಯದಿ (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  ಪ್ರತಿ (hb_codepoint_t gid = 0; gid < ಅಂಕೆ_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    ಯದಿ (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    ಅನ್ಯಥಾ
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((ಧ್ರುವ ವರ್ಣ*) ustr.arrayZ, ustr.length);
	    }
	    ಯದಿ (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	ಯದಿ (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  ಯದಿ (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  ಗಚ್ಛ retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      ಧ್ರುವ gname_t *gname = names ? names->bsearch (key) : nullptr;
      ಯದಿ (!gname) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      ಯದಿ (!gid && gname->sid) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      *glyph = gid;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    HB_INTERNAL ತಾರ್ಕಿಕ get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) ಧ್ರುವ;
    HB_INTERNAL ತಾರ್ಕಿಕ get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ಧ್ರುವ;

    ಗುಪ್ತ:
    ಸಂರಚನಾ gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *a_, ಧ್ರುವ ಶೂನ್ಯ *b_)
      {
	ಧ್ರುವ gname_t *a = (ಧ್ರುವ gname_t *)a_;
	ಧ್ರುವ gname_t *b = (ಧ್ರುವ gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	ಪೂರ್ಣಾಙ್ಕ ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	ಯದಿ (ret) ಪ್ರತ್ಯಾಗಮ ret;
	ಪ್ರತ್ಯಾಗಮ a->name.length - b->name.length;
      }

      ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ gname_t &a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ cmp (&a, ಏತತ್); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    ಪ್ರಕಾರನಿರ್ದೇಶ accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_ಮೌಲ್ಯಗಳು_t> SUPER;
  };

  ಸಂರಚನಾ accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_ಮೌಲ್ಯಗಳು_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ಯದಿ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ;
    HB_INTERNAL ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
				ಸಂರಚನಾ cff1_subset_plan &plan) ಧ್ರುವ;
    HB_INTERNAL ತಾರ್ಕಿಕ get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) ಧ್ರುವ;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    ಪ್ರಕಾರನಿರ್ದೇಶ accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_ಮೌಲ್ಯಗಳು_subset_t> SUPER;
  };

  ರಕ್ಷಿತ:
  HB_INTERNAL ಸ್ಥಿರ hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL ಸ್ಥಿರ hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL ಸ್ಥಿರ hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL ಸ್ಥಿರ hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL ಸ್ಥಿರ hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL ಸ್ಥಿರ hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL ಸ್ಥಿರ hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  ಸಾರ್ವಜನಿಕ:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

ಸಂರಚನಾ cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  ಶೂನ್ಯ depend (hb_depend_data_builder_t *builder) ಧ್ರುವ
  {
    ಯದಿ (!is_valid ()) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ (hb_codepoint_t gid = 0; gid < ಅಂಕೆ_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      ಯದಿ (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
