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

#अन्तर्भाव "hb-ot-cff-common.hh"
#अन्तर्भाव "hb-depend-data.hh"
#अन्तर्भाव "hb-subset-cff-common.hh"
#अन्तर्भाव "hb-draw.hh"
#अन्तर्भाव "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#अन्तर्भाव "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

नामाकाश CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

गणना EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
गणना CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

प्रकारनिर्देश CFF1Index          CFF1CharStrings;
प्रकारनिर्देश Subrs<HBUINT16>    CFF1Subrs;

संरचना CFF1FDSelect : FDSelect {};

/* Encoding */
संरचना Encoding0 {
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ध्रुव
  {
    प्रतिज्ञा (glyph > 0);
    glyph--;
    यदि (glyph < nCodes ())
    {
      प्रत्यागम (hb_codepoint_t)codes[glyph];
    }
    अन्यथा
      प्रत्यागम CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { प्रत्यागम codes.len; }
  HBUINT8 nCodes () ध्रुव { प्रत्यागम codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

संरचना Encoding1_Range {
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT8   first;
  HBUINT8   n_डावा;

  DEFINE_SIZE_STATIC (2);
};

संरचना Encoding1 {
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ध्रुव
  {
    /* TODO: Add cache like get_sid. */
    प्रतिज्ञा (glyph > 0);
    glyph--;
    प्रति (unsigned पूर्णाङ्क i = 0; i < nRanges (); i++)
    {
      यदि (glyph <= ranges[i].n_डावा)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	प्रत्यागम (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_डावा + 1);
    }
    प्रत्यागम CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { प्रत्यागम ranges.len; }
  HBUINT8 nRanges () ध्रुव { प्रत्यागम ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

संरचना SuppEncoding {
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

संरचना CFF1SuppEncData {
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (supps.sanitize (c));
  }

  शून्य get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ध्रुव
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < nSups (); i++)
      यदि (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { प्रत्यागम supps.len; }
  HBUINT8 nSups () ध्रुव { प्रत्यागम supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

संरचना Encoding
{
  /* serialize a fullset Encoding */
  तार्किक serialize (hb_serialize_context_t *c, ध्रुव Encoding &src)
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  तार्किक serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned पूर्णाङ्क enc_मोजणी,
		  ध्रुव hb_vector_t<code_pair_t>& code_ranges,
		  ध्रुव hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (एतत्);
    Encoding *dest = c->extend_min (एतत्);
    यदि (unlikely (!dest)) return_trace (असत्य);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    विकल्प (format) {
    पक्ष 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_मोजणी);
      यदि (unlikely (!fmt0)) return_trace (असत्य);
      fmt0->nCodes () = enc_मोजणी;
      unsigned पूर्णाङ्क glyph = 0;
      प्रति (unsigned पूर्णाङ्क i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	प्रति (पूर्णाङ्क left = (पूर्णाङ्क)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	यदि (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (असत्य);
      }
    }
    भङ्ग;

    पक्ष 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      यदि (unlikely (!fmt1)) return_trace (असत्य);
      fmt1->nRanges () = code_ranges.length;
      प्रति (unsigned पूर्णाङ्क i = 0; i < code_ranges.length; i++)
      {
	यदि (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (असत्य);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_डावा = code_ranges[i].glyph;
      }
    }
    भङ्ग;

    }

    यदि (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      यदि (unlikely (!suppData)) return_trace (असत्य);
      suppData->nSups () = supp_codes.length;
      प्रति (unsigned पूर्णाङ्क i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (सत्य);
  }

  size_t get_size () ध्रुव
  {
    size_t size = min_size;
    विकल्प (table_format ())
    {
    पक्ष 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); भङ्ग;
    पक्ष 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); भङ्ग;
    }
    यदि (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    प्रत्यागम size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ध्रुव
  {
    विकल्प (table_format ())
    {
    पक्ष 0: hb_barrier (); प्रत्यागम u.format0.get_code (glyph);
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_code (glyph);
    स्वभाव:प्रत्यागम 0;
    }
  }

  uint8_t table_format () ध्रुव { प्रत्यागम format & 0x7F; }
  तार्किक  has_supplement () ध्रुव { प्रत्यागम format & 0x80; }

  शून्य get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ध्रुव
  {
    codes.clear ();
    यदि (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्)))
      return_trace (असत्य);
    hb_barrier ();

    विकल्प (table_format ())
    {
    पक्ष 0: hb_barrier (); यदि (unlikely (!u.format0.sanitize (c))) { return_trace (असत्य); } भङ्ग;
    पक्ष 1: hb_barrier (); यदि (unlikely (!u.format1.sanitize (c))) { return_trace (असत्य); } भङ्ग;
    स्वभाव:return_trace (असत्य);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  रक्षित:
  ध्रुव CFF1SuppEncData &suppEncData () ध्रुव
  {
    विकल्प (table_format ())
    {
    पक्ष 0: hb_barrier (); प्रत्यागम StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    पक्ष 1: hb_barrier (); प्रत्यागम StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    स्वभाव:प्रत्यागम Null (CFF1SuppEncData);
    }
  }

  सार्वजनिक:
  HBUINT8	format;
  सङ्घ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
संरचना Charset0
{
  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क अंक_glyphs, unsigned *अंक_charset_entries) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (अंक_charset_entries) *अंक_charset_entries = अंक_glyphs;
    return_trace (sids.sanitize (c, अंक_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned अंक_glyphs) ध्रुव
  {
    यदि (unlikely (glyph >= अंक_glyphs)) प्रत्यागम 0;
    यदि (unlikely (glyph == 0))
      प्रत्यागम 0;
    अन्यथा
      प्रत्यागम sids[glyph - 1];
  }

  शून्य collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned पूर्णाङ्क अंक_glyphs) ध्रुव
  {
    यदि (!mapping->resize_dirty  (अंक_glyphs)) प्रत्यागम;
    प्रति (hb_codepoint_t gid = 1; gid < अंक_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned पूर्णाङ्क अंक_glyphs) ध्रुव
  {
    यदि (sid == 0)
      प्रत्यागम 0;

    प्रति (unsigned पूर्णाङ्क glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      यदि (sids[glyph-1] == sid)
	प्रत्यागम glyph;
    }
    प्रत्यागम 0;
  }

  स्थिर size_t get_size (unsigned पूर्णाङ्क अंक_glyphs)
  {
    प्रतिज्ञा (अंक_glyphs > 0);
    प्रत्यागम UnsizedArrayOf<HBUINT16>::get_size (अंक_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
संरचना Charset_Range {
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT16  first;
  TYPE      n_डावा;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
संरचना Charset1_2 {
  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क अंक_glyphs, unsigned *अंक_charset_entries) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    अंक_glyphs--;
    unsigned i;
    प्रति (i = 0; अंक_glyphs > 0; i++)
    {
      यदि (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (अंक_glyphs >= ranges[i].n_डावा + 1))))
	return_trace (असत्य);
      अंक_glyphs -= (ranges[i].n_डावा + 1);
    }
    यदि (अंक_charset_entries)
      *अंक_charset_entries = i;
    return_trace (सत्य);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned अंक_glyphs,
			  code_pair_t *cache = nullptr) ध्रुव
  {
    यदि (unlikely (glyph >= अंक_glyphs)) प्रत्यागम 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    यदि (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    अन्यथा
    {
      यदि (unlikely (glyph == 0)) प्रत्यागम 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    प्रति (;; i++)
    {
      unsigned मोजणी = ranges[i].n_डावा;
      यदि (glyph <= मोजणी)
      {
        यदि (cache)
	  *cache = {i, start_glyph};
	प्रत्यागम ranges[i].first + glyph;
      }
      मोजणी++;
      start_glyph += मोजणी;
      glyph -= मोजणी;
    }

    प्रत्यागम 0;
  }

  शून्य collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned पूर्णाङ्क अंक_glyphs) ध्रुव
  {
    यदि (!mapping->resize_dirty  (अंक_glyphs)) प्रत्यागम;

    hb_codepoint_t gid = 1;
    यदि (gid >= अंक_glyphs)
      प्रत्यागम;

    प्रति (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned मोजणी = ranges[i].n_डावा + 1;
      unsigned last = gid + मोजणी;
      प्रति (unsigned j = 0; j < मोजणी; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      यदि (gid >= अंक_glyphs)
        भङ्ग;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned पूर्णाङ्क अंक_glyphs) ध्रुव
  {
    यदि (sid == 0) प्रत्यागम 0;
    hb_codepoint_t  glyph = 1;
    प्रति (unsigned पूर्णाङ्क i = 0;; i++)
    {
      यदि (glyph >= अंक_glyphs)
	प्रत्यागम 0;
      यदि ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_डावा))
	प्रत्यागम glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_डावा + 1);
    }

    प्रत्यागम 0;
  }

  size_t get_size (unsigned पूर्णाङ्क अंक_glyphs) ध्रुव
  {
    पूर्णाङ्क glyph = (पूर्णाङ्क) अंक_glyphs;
    unsigned अंक_ranges = 0;

    प्रतिज्ञा (glyph > 0);
    glyph--;
    प्रति (unsigned पूर्णाङ्क i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_डावा + 1);
      अंक_ranges++;
    }

    प्रत्यागम get_size_for_ranges (अंक_ranges);
  }

  स्थिर size_t get_size_for_ranges (unsigned पूर्णाङ्क अंक_ranges)
  {
    प्रत्यागम UnsizedArrayOf<Charset_Range<TYPE> >::get_size (अंक_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

प्रकारनिर्देश Charset1_2<HBUINT8>     Charset1;
प्रकारनिर्देश Charset1_2<HBUINT16>    Charset2;
प्रकारनिर्देश Charset_Range<HBUINT8>  Charset1_Range;
प्रकारनिर्देश Charset_Range<HBUINT16> Charset2_Range;

संरचना Charset
{
  /* serialize a fullset Charset */
  तार्किक serialize (hb_serialize_context_t *c, ध्रुव Charset &src, unsigned पूर्णाङ्क अंक_glyphs)
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed ((ध्रुव वर्ण *) &src, src.get_size (अंक_glyphs)));
  }

  /* serialize a subset Charset */
  तार्किक serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned पूर्णाङ्क अंक_glyphs,
		  ध्रुव hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (एतत्);
    Charset *dest = c->extend_min (एतत्);
    यदि (unlikely (!dest)) return_trace (असत्य);
    dest->format = format;
    विकल्प (format)
    {
    पक्ष 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (अंक_glyphs), असत्य);
      यदि (unlikely (!fmt0)) return_trace (असत्य);
      unsigned पूर्णाङ्क glyph = 0;
      प्रति (unsigned पूर्णाङ्क i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	प्रति (पूर्णाङ्क left = (पूर्णाङ्क)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    भङ्ग;

    पक्ष 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), असत्य);
      यदि (unlikely (!fmt1)) return_trace (असत्य);
      hb_codepoint_t all_glyphs = 0;
      प्रति (unsigned पूर्णाङ्क i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_डावा = _.glyph;
      }
      यदि (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (असत्य);
    }
    भङ्ग;

    पक्ष 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), असत्य);
      यदि (unlikely (!fmt2)) return_trace (असत्य);
      hb_codepoint_t all_glyphs = 0;
      प्रति (unsigned पूर्णाङ्क i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_डावा = _.glyph;
      }
      यदि (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (असत्य);
    }
    भङ्ग;

    }
    return_trace (सत्य);
  }

  size_t get_size (unsigned पूर्णाङ्क अंक_glyphs) ध्रुव
  {
    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); प्रत्यागम hb_unsigned_add_saturate (min_size, u.format0.get_size (अंक_glyphs));
    पक्ष 1: hb_barrier (); प्रत्यागम hb_unsigned_add_saturate (min_size, u.format1.get_size (अंक_glyphs));
    पक्ष 2: hb_barrier (); प्रत्यागम hb_unsigned_add_saturate (min_size, u.format2.get_size (अंक_glyphs));
    स्वभाव:प्रत्यागम 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned पूर्णाङ्क अंक_glyphs,
			  code_pair_t *cache = nullptr) ध्रुव
  {
    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); प्रत्यागम u.format0.get_sid (glyph, अंक_glyphs);
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_sid (glyph, अंक_glyphs, cache);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.get_sid (glyph, अंक_glyphs, cache);
    स्वभाव:प्रत्यागम 0;
    }
  }

  शून्य collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned पूर्णाङ्क अंक_glyphs) ध्रुव
  {
    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, अंक_glyphs); प्रत्यागम;
    पक्ष 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, अंक_glyphs); प्रत्यागम;
    पक्ष 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, अंक_glyphs); प्रत्यागम;
    स्वभाव:प्रत्यागम;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned पूर्णाङ्क अंक_glyphs) ध्रुव
  {
    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); प्रत्यागम u.format0.get_glyph (sid, अंक_glyphs);
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_glyph (sid, अंक_glyphs);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.get_glyph (sid, अंक_glyphs);
    स्वभाव:प्रत्यागम 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c, unsigned *अंक_charset_entries) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्)))
      return_trace (असत्य);
    hb_barrier ();

    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    स्वभाव:return_trace (असत्य);
    }
  }

  HBUINT8       format;
  सङ्घ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

संरचना CFF1StringIndex : CFF1Index
{
  तार्किक serialize (hb_serialize_context_t *c, ध्रुव CFF1StringIndex &strings,
		  ध्रुव hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely ((strings.मोजणी == 0) || (sidmap.length == 0)))
    {
      यदि (unlikely (!c->extend_min (एतत्->मोजणी)))
	return_trace (असत्य);
      मोजणी = 0;
      return_trace (सत्य);
    }

    यदि (unlikely (sidmap.in_error ())) return_trace (असत्य);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    यदि (unlikely (bytesArray.in_error ())) return_trace (असत्य);

    तार्किक result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

संरचना cff1_शीर्ष_dict_interp_env_t : अंक_interp_env_t
{
  cff1_शीर्ष_dict_interp_env_t ()
    : अंक_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_शीर्ष_dict_interp_env_t (ध्रुव hb_ubytes_t &bytes)
    : अंक_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned पूर्णाङ्क prev_offset;
  unsigned पूर्णाङ्क last_offset;
};

संरचना name_dict_मूल्ये_t
{
  गणना name_dict_val_index_t
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

      Val_मोजणी
  };

  शून्य init ()
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < Val_मोजणी; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned पूर्णाङ्क& operator[] (unsigned पूर्णाङ्क i)
  { प्रतिज्ञा (i < Val_मोजणी); प्रत्यागम values[i]; }

  unsigned पूर्णाङ्क operator[] (unsigned पूर्णाङ्क i) ध्रुव
  { प्रतिज्ञा (i < Val_मोजणी); प्रत्यागम values[i]; }

  स्थिर गणना name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    विकल्प (op) {
      स्वभाव: // can't happen - just make some compiler happy
      पक्ष OpCode_version:
	प्रत्यागम version;
      पक्ष OpCode_Notice:
	प्रत्यागम notice;
      पक्ष OpCode_Copyright:
	प्रत्यागम copyright;
      पक्ष OpCode_FullName:
	प्रत्यागम fullName;
      पक्ष OpCode_FamilyName:
	प्रत्यागम familyName;
      पक्ष OpCode_Weight:
	प्रत्यागम weight;
      पक्ष OpCode_PostScript:
	प्रत्यागम postscript;
      पक्ष OpCode_FontName:
	प्रत्यागम fontName;
      पक्ष OpCode_BaseFontName:
	प्रत्यागम baseFontName;
    }
  }

  unsigned पूर्णाङ्क  values[Val_मोजणी];
};

संरचना cff1_शीर्ष_dict_val_t : op_str_t
{
  unsigned पूर्णाङ्क  last_arg_offset;
};

संरचना cff1_शीर्ष_dict_मूल्ये_t : शीर्ष_dict_मूल्ये_t<cff1_शीर्ष_dict_val_t>
{
  शून्य init ()
  {
    शीर्ष_dict_मूल्ये_t<cff1_शीर्ष_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_मोजणी = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  शून्य fini () { शीर्ष_dict_मूल्ये_t<cff1_शीर्ष_dict_val_t>::fini (); }

  तार्किक is_CID () ध्रुव
  { प्रत्यागम nameSIDs[name_dict_मूल्ये_t::registry] != CFF_UNDEF_SID; }

  name_dict_मूल्ये_t  nameSIDs;
  unsigned पूर्णाङ्क    ros_supplement_offset;
  unsigned पूर्णाङ्क    ros_supplement;
  unsigned पूर्णाङ्क    cid_मोजणी;

  पूर्णाङ्क             EncodingOffset;
  पूर्णाङ्क             CharsetOffset;
  पूर्णाङ्क             FDSelectOffset;
  table_info_t    privateDictInfo;
};

संरचना cff1_शीर्ष_dict_opset_t : शीर्ष_dict_opset_t<cff1_शीर्ष_dict_val_t>
{
  स्थिर शून्य process_op (op_code_t op, cff1_शीर्ष_dict_interp_env_t& env, cff1_शीर्ष_dict_मूल्ये_t& dictval)
  {
    cff1_शीर्ष_dict_val_t  val {};

    विकल्प (op) {
      पक्ष OpCode_version:
      पक्ष OpCode_Notice:
      पक्ष OpCode_Copyright:
      पक्ष OpCode_FullName:
      पक्ष OpCode_FontName:
      पक्ष OpCode_FamilyName:
      पक्ष OpCode_Weight:
      पक्ष OpCode_PostScript:
      पक्ष OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_मूल्ये_t::name_op_to_index (op)] = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_isFixedPitch:
      पक्ष OpCode_ItalicAngle:
      पक्ष OpCode_UnderlinePosition:
      पक्ष OpCode_UnderlineThickness:
      पक्ष OpCode_PaintType:
      पक्ष OpCode_CharstringType:
      पक्ष OpCode_UniqueID:
      पक्ष OpCode_StrokeWidth:
      पक्ष OpCode_SyntheticBase:
      पक्ष OpCode_CIDFontVersion:
      पक्ष OpCode_CIDFontRevision:
      पक्ष OpCode_CIDFontType:
      पक्ष OpCode_UIDBase:
      पक्ष OpCode_FontBBox:
      पक्ष OpCode_XUID:
      पक्ष OpCode_BaseFontBlend:
	env.clear_args ();
	भङ्ग;

      पक्ष OpCode_CIDCount:
	dictval.cid_मोजणी = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	भङ्ग;

      पक्ष OpCode_ROS:
	dictval.ros_supplement = env.arg_चळत.काढा_uint ();
	dictval.nameSIDs[name_dict_मूल्ये_t::ordering] = env.arg_चळत.काढा_uint ();
	dictval.nameSIDs[name_dict_मूल्ये_t::registry] = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	यदि (unlikely (env.in_error ())) प्रत्यागम;
	प्रतिज्ञा (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	भङ्ग;

      पक्ष OpCode_Encoding:
	dictval.EncodingOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	यदि (unlikely (dictval.EncodingOffset == 0)) प्रत्यागम;
	भङ्ग;

      पक्ष OpCode_charset:
	dictval.CharsetOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	यदि (unlikely (dictval.CharsetOffset == 0)) प्रत्यागम;
	भङ्ग;

      पक्ष OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	भङ्ग;

      पक्ष OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_चळत.काढा_int ();
	dictval.privateDictInfo.size = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	भङ्ग;

      स्वभाव:
	env.last_offset = env.str_ref.get_offset ();
	शीर्ष_dict_opset_t<cff1_शीर्ष_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	यदि (!env.arg_चळत.is_empty ()) प्रत्यागम;
	भङ्ग;
    }

    यदि (unlikely (env.in_error ())) प्रत्यागम;

    dictval.add_op (op, env.str_ref, val);
  }
};

संरचना cff1_font_dict_मूल्ये_t : dict_मूल्ये_t<op_str_t>
{
  शून्य init ()
  {
    dict_मूल्ये_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  शून्य fini () { dict_मूल्ये_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned पूर्णाङ्क    fontName;
};

संरचना cff1_font_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, अंक_interp_env_t& env, cff1_font_dict_मूल्ये_t& dictval)
  {
    विकल्प (op) {
      पक्ष OpCode_FontName:
	dictval.fontName = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_FontMatrix:
      पक्ष OpCode_PaintType:
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_चळत.काढा_uint ();
	dictval.privateDictInfo.size = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	भङ्ग;

      स्वभाव:
	dict_opset_t::process_op (op, env);
	यदि (!env.arg_चळत.is_empty ()) प्रत्यागम;
	भङ्ग;
    }

    यदि (unlikely (env.in_error ())) प्रत्यागम;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
संरचना cff1_private_dict_मूल्ये_base_t : dict_मूल्ये_t<VAL>
{
  शून्य init ()
  {
    dict_मूल्ये_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  शून्य fini () { dict_मूल्ये_t<VAL>::fini (); }

  पूर्णाङ्क                 subrsOffset;
  ध्रुव CFF1Subrs    *localSubrs;
};

प्रकारनिर्देश cff1_private_dict_मूल्ये_base_t<op_str_t> cff1_private_dict_मूल्ये_subset_t;
प्रकारनिर्देश cff1_private_dict_मूल्ये_base_t<अंक_dict_val_t> cff1_private_dict_मूल्ये_t;

संरचना cff1_private_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, अंक_interp_env_t& env, cff1_private_dict_मूल्ये_t& dictval)
  {
    अंक_dict_val_t val;
    val.init ();

    विकल्प (op) {
      पक्ष Op_Code_Blue_मूल्ये:
      पक्ष OpCode_OtherBlues:
      पक्ष OpCode_FamilyBlues:
      पक्ष OpCode_FamilyOtherBlues:
      पक्ष OpCode_StemSnapH:
      पक्ष OpCode_StemSnapV:
      पक्ष OpCode_StdHW:
      पक्ष OpCode_StdVW:
      पक्ष OpCode_BlueScale:
      पक्ष OpCode_BlueShift:
      पक्ष OpCode_BlueFuzz:
      पक्ष OpCode_ForceBold:
      पक्ष OpCode_LanguageGroup:
      पक्ष OpCode_ExpansionFactor:
      पक्ष OpCode_initialRandomSeed:
      पक्ष OpCode_defaultWidthX:
      पक्ष OpCode_nominalWidthX:
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_Subrs:
	dictval.subrsOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	भङ्ग;

      स्वभाव:
	dict_opset_t::process_op (op, env);
	यदि (!env.arg_चळत.is_empty ()) प्रत्यागम;
	भङ्ग;
    }

    यदि (unlikely (env.in_error ())) प्रत्यागम;

    dictval.add_op (op, env.str_ref, val);
  }
};

संरचना cff1_private_dict_opset_subset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, अंक_interp_env_t& env, cff1_private_dict_मूल्ये_subset_t& dictval)
  {
    विकल्प (op) {
      पक्ष Op_Code_Blue_मूल्ये:
      पक्ष OpCode_OtherBlues:
      पक्ष OpCode_FamilyBlues:
      पक्ष OpCode_FamilyOtherBlues:
      पक्ष OpCode_StemSnapH:
      पक्ष OpCode_StemSnapV:
      पक्ष OpCode_StdHW:
      पक्ष OpCode_StdVW:
      पक्ष OpCode_BlueScale:
      पक्ष OpCode_BlueShift:
      पक्ष OpCode_BlueFuzz:
      पक्ष OpCode_ForceBold:
      पक्ष OpCode_LanguageGroup:
      पक्ष OpCode_ExpansionFactor:
      पक्ष OpCode_initialRandomSeed:
      पक्ष OpCode_defaultWidthX:
      पक्ष OpCode_nominalWidthX:
	env.clear_args ();
	भङ्ग;

      पक्ष OpCode_Subrs:
	dictval.subrsOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	भङ्ग;

      स्वभाव:
	dict_opset_t::process_op (op, env);
	यदि (!env.arg_चळत.is_empty ()) प्रत्यागम;
	भङ्ग;
    }

    यदि (unlikely (env.in_error ())) प्रत्यागम;

    dictval.add_op (op, env.str_ref);
  }
};

प्रकारनिर्देश dict_interpreter_t<cff1_शीर्ष_dict_opset_t, cff1_शीर्ष_dict_मूल्ये_t, cff1_शीर्ष_dict_interp_env_t> cff1_शीर्ष_dict_interpreter_t;
प्रकारनिर्देश dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_मूल्ये_t> cff1_font_dict_interpreter_t;

प्रकारनिर्देश CFF1Index CFF1NameIndex;
प्रकारनिर्देश CFF1Index CFF1_शीर्ष_Dict_Index;

संरचना cff1_font_dict_मूल्ये_mod_t
{
  cff1_font_dict_मूल्ये_mod_t() { init (); }

  शून्य init () { init ( &Null (cff1_font_dict_मूल्ये_t), CFF_UNDEF_SID ); }

  शून्य init (ध्रुव cff1_font_dict_मूल्ये_t *base_,
	     unsigned पूर्णाङ्क fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_मोजणी () ध्रुव { प्रत्यागम base->get_मोजणी (); }

  ध्रुव op_str_t &operator [] (unsigned पूर्णाङ्क i) ध्रुव { प्रत्यागम (*base)[i]; }

  ध्रुव cff1_font_dict_मूल्ये_t    *base;
  table_info_t		   privateDictInfo;
  unsigned पूर्णाङ्क		fontName;
};

संरचना CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  तार्किक serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { प्रत्यागम FDArray<HBUINT16>::serialize<cff1_font_dict_मूल्ये_mod_t, cff1_font_dict_मूल्ये_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

नामाकाश OT {

using नामाकाश CFF;

संरचना cff1
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  संरचना accelerator_templ_t
  {
    स्थिर constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      यदि (!face) प्रत्यागम;

      शीर्ष_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      एतत्->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (एतत्->blob);
      sc.start_processing ();

      ध्रुव OT::cff1 *cff = एतत्->blob->template as<OT::cff1> ();

      यदि (cff == &Null (OT::cff1))
        गच्छ fail;

      nameIndex = &cff->nameIndex (cff);
      यदि ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        गच्छ fail;
      hb_barrier ();

      शीर्ष_Dict_Index = &StructAtOffsetOrNull<CFF1_शीर्ष_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      यदि (शीर्ष_Dict_Index == &Null (CFF1_शीर्ष_Dict_Index) || (शीर्ष_Dict_Index->मोजणी == 0))
        गच्छ fail;
      hb_barrier ();

      { /* parse top dict */
	ध्रुव hb_ubytes_t शीर्ष_Dict_Str = (*शीर्ष_Dict_Index)[0];
	यदि (unlikely (!शीर्ष_Dict_Str.sanitize (&sc)))   गच्छ fail;
	hb_barrier ();
	cff1_शीर्ष_dict_interp_env_t env (शीर्ष_Dict_Str);
	cff1_शीर्ष_dict_interpreter_t शीर्ष_interp (env);
	यदि (unlikely (!शीर्ष_interp.interpret (शीर्ष_Dict_2)))   गच्छ fail;
      }

      यदि (is_predef_charset ())
	charset = &Null (Charset);
      अन्यथा
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, शीर्ष_Dict_2.CharsetOffset, sc, &अंक_charset_entries);
	यदि (unlikely (charset == &Null (Charset)))   गच्छ fail;
      }

      fd_मोजणी = 1;
      यदि (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, शीर्ष_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, शीर्ष_Dict_2.FDSelectOffset, sc, fdArray->मोजणी);
	यदि (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  गच्छ fail;

	fd_मोजणी = fdArray->मोजणी;
      }
      अन्यथा
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      यदि (is_CID ())
      {
	यदि (unlikely (charset == &Null (Charset)))   गच्छ fail;
      }
      अन्यथा
      {
	यदि (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, शीर्ष_Dict_2.EncodingOffset, sc);
	  यदि (unlikely (encoding == &Null (Encoding)))   गच्छ fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (शीर्ष_Dict_Index, शीर्ष_Dict_Index->get_size (), sc);
      यदि (stringIndex == &Null (CFF1StringIndex))
        गच्छ fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, शीर्ष_Dict_2.charStringsOffset, sc);
      यदि (charStrings == &Null (CFF1CharStrings))
        गच्छ fail;

      अंक_glyphs = charStrings->मोजणी;
      यदि (अंक_glyphs != sc.get_अंक_glyphs ())
        गच्छ fail;

      यदि (unlikely (!privateDicts.resize (fd_मोजणी)))
        गच्छ fail;
      प्रति (unsigned पूर्णाङ्क i = 0; i < fd_मोजणी; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      यदि (is_CID ())
      {
	प्रति (unsigned पूर्णाङ्क i = 0; i < fd_मोजणी; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  यदि (unlikely (!fontDictStr.sanitize (&sc)))   गच्छ fail;
	  hb_barrier ();
	  cff1_font_dict_मूल्ये_t *font;
	  cff1_शीर्ष_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  यदि (unlikely (fontDicts.in_error ()))   गच्छ fail;

	  font->init ();
	  यदि (unlikely (!font_interp.interpret (*font)))   गच्छ fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  ध्रुव hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  यदि (unlikely (font->privateDictInfo.size &&
			privDictStr == (ध्रुव unsigned वर्ण *) &Null (UnsizedByteStr))) गच्छ fail;
	  अंक_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  यदि (unlikely (!priv_interp.interpret (*priv)))   गच्छ fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      अन्यथा  /* non-CID */
      {
	cff1_शीर्ष_dict_मूल्ये_t *font = &शीर्ष_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	ध्रुव hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	यदि (font->privateDictInfo.size &&
	    unlikely (privDictStr == (ध्रुव unsigned वर्ण *) &Null (UnsizedByteStr))) गच्छ fail;
	अंक_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	यदि (unlikely (!priv_interp.interpret (*priv)))   गच्छ fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      प्रत्यागम;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    शून्य _fini ()
    {
      sc.end_processing ();
      शीर्ष_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () ध्रुव { प्रत्यागम blob; }

    तार्किक is_valid () ध्रुव { प्रत्यागम blob; }
    तार्किक   is_CID () ध्रुव { प्रत्यागम शीर्ष_Dict_2.is_CID (); }

    तार्किक is_predef_charset () ध्रुव { प्रत्यागम शीर्ष_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned पूर्णाङ्क std_code_to_glyph (hb_codepoint_t code) ध्रुव
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      यदि (unlikely (sid == CFF_UNDEF_SID))
	प्रत्यागम 0;

      यदि (charset != &Null (Charset))
	प्रत्यागम charset->get_glyph (sid, अंक_glyphs);
      अन्यथा यदि ((शीर्ष_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) प्रत्यागम sid;
      प्रत्यागम 0;
    }

    तार्किक is_predef_encoding () ध्रुव { प्रत्यागम शीर्ष_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) ध्रुव
    {
      यदि (encoding != &Null (Encoding))
	प्रत्यागम encoding->get_code (glyph);
      अन्यथा
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	यदि (sid == 0) प्रत्यागम 0;
	hb_codepoint_t code = 0;
	विकल्प (शीर्ष_Dict_2.EncodingOffset)
	{
	पक्ष StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  भङ्ग;
	पक्ष ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  भङ्ग;
	स्वभाव:
	  भङ्ग;
	}
	प्रत्यागम code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () ध्रुव
    {
      यदि (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (परिमाण (glyph_to_sid_map_t));
	यदि (unlikely (!mapping)) प्रत्यागम nullptr;
	mapping = नूतन (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, अंक_glyphs);
	प्रत्यागम mapping;
      }
      अन्यथा
	प्रत्यागम nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) ध्रुव
    {
      यदि (charset != &Null (Charset))
	प्रत्यागम charset->get_sid (glyph, अंक_glyphs, cache);
      अन्यथा
      {
	hb_codepoint_t sid = 0;
	विकल्प (शीर्ष_Dict_2.CharsetOffset)
	{
	  पक्ष ISOAdobeCharset:
	    यदि (glyph <= 228 /*zcaron*/) sid = glyph;
	    भङ्ग;
	  पक्ष ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    भङ्ग;
	  पक्ष ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    भङ्ग;
	  स्वभाव:
	    भङ्ग;
	}
	प्रत्यागम sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) ध्रुव
    {
      यदि (charset != &Null (Charset))
	प्रत्यागम charset->get_glyph (sid, अंक_glyphs);
      अन्यथा
      {
	hb_codepoint_t glyph = 0;
	विकल्प (शीर्ष_Dict_2.CharsetOffset)
	{
	  पक्ष ISOAdobeCharset:
	    यदि (sid <= 228 /*zcaron*/) glyph = sid;
	    भङ्ग;
	  पक्ष ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    भङ्ग;
	  पक्ष ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    भङ्ग;
	  स्वभाव:
	    भङ्ग;
	}
	प्रत्यागम glyph;
      }
    }

    रक्षित:
    hb_sanitize_context_t   sc;

    सार्वजनिक:
    hb_blob_t               *blob = nullptr;
    ध्रुव Encoding	    *encoding = nullptr;
    ध्रुव Charset	    *charset = nullptr;
    ध्रुव CFF1NameIndex     *nameIndex = nullptr;
    ध्रुव CFF1_शीर्ष_Dict_Index  *शीर्ष_Dict_Index = nullptr;
    ध्रुव CFF1StringIndex   *stringIndex = nullptr;
    ध्रुव CFF1Subrs	    *globalSubrs = nullptr;
    ध्रुव CFF1CharStrings   *charStrings = nullptr;
    ध्रुव CFF1FDArray       *fdArray = nullptr;
    ध्रुव CFF1FDSelect      *fdSelect = nullptr;
    unsigned पूर्णाङ्क	     fd_मोजणी = 0;

    cff1_शीर्ष_dict_मूल्ये_t   शीर्ष_Dict_2;
    hb_vector_t<cff1_font_dict_मूल्ये_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned पूर्णाङ्क	     अंक_glyphs = 0;
    unsigned पूर्णाङ्क	     अंक_charset_entries = 0;
  };

  संरचना accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_मूल्ये_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      यदि (!is_valid ()) प्रत्यागम;
      यदि (is_CID ()) प्रत्यागम;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      यदि (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    तार्किक get_glyph_name (hb_codepoint_t glyph,
			 वर्ण *buf, unsigned पूर्णाङ्क buf_len) ध्रुव
    {
      यदि (unlikely (glyph >= अंक_glyphs)) प्रत्यागम असत्य;
      यदि (unlikely (!is_valid ())) प्रत्यागम असत्य;
      यदि (is_CID()) प्रत्यागम असत्य;
      यदि (unlikely (!buf_len)) प्रत्यागम सत्य;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      ध्रुव वर्ण *str;
      size_t str_len;
      यदि (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      अन्यथा
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (ध्रुव वर्ण *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      यदि (!str_len) प्रत्यागम असत्य;
      unsigned पूर्णाङ्क len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ध्रुव वर्ण*)str, len);
      buf[len] = '\0';
      प्रत्यागम सत्य;
    }

    तार्किक get_glyph_from_name (ध्रुव वर्ण *name, पूर्णाङ्क len,
			      hb_codepoint_t *glyph) ध्रुव
    {
      यदि (unlikely (!is_valid ())) प्रत्यागम असत्य;
      यदि (is_CID()) प्रत्यागम असत्य;
      यदि (len < 0) len = strlen (name);
      यदि (unlikely (!len)) प्रत्यागम असत्य;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      यदि (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, परिमाण (hb_sorted_vector_t<gname_t>));
	यदि (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  प्रति (hb_codepoint_t gid = 0; gid < अंक_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    यदि (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    अन्यथा
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((ध्रुव वर्ण*) ustr.arrayZ, ustr.length);
	    }
	    यदि (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	यदि (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  यदि (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  गच्छ retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      ध्रुव gname_t *gname = names ? names->bsearch (key) : nullptr;
      यदि (!gname) प्रत्यागम असत्य;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      यदि (!gid && gname->sid) प्रत्यागम असत्य;
      *glyph = gid;
      प्रत्यागम सत्य;
    }

    HB_INTERNAL तार्किक get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) ध्रुव;
    HB_INTERNAL तार्किक get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ध्रुव;

    गुप्त:
    संरचना gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *a_, ध्रुव शून्य *b_)
      {
	ध्रुव gname_t *a = (ध्रुव gname_t *)a_;
	ध्रुव gname_t *b = (ध्रुव gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	पूर्णाङ्क ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	यदि (ret) प्रत्यागम ret;
	प्रत्यागम a->name.length - b->name.length;
      }

      पूर्णाङ्क cmp (ध्रुव gname_t &a) ध्रुव { प्रत्यागम cmp (&a, एतत्); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    प्रकारनिर्देश accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_मूल्ये_t> SUPER;
  };

  संरचना accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_मूल्ये_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      यदि (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL तार्किक subset (hb_subset_context_t *c) ध्रुव;
    HB_INTERNAL तार्किक serialize (hb_serialize_context_t *c,
				संरचना cff1_subset_plan &plan) ध्रुव;
    HB_INTERNAL तार्किक get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) ध्रुव;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    प्रकारनिर्देश accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_मूल्ये_subset_t> SUPER;
  };

  रक्षित:
  HB_INTERNAL स्थिर hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL स्थिर hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL स्थिर hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL स्थिर hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL स्थिर hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL स्थिर hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL स्थिर hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  सार्वजनिक:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

संरचना cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  शून्य depend (hb_depend_data_builder_t *builder) ध्रुव
  {
    यदि (!is_valid ()) प्रत्यागम;
    प्रति (hb_codepoint_t gid = 0; gid < अंक_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      यदि (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
