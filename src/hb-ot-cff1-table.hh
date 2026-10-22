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

प्रकारव्याख्या CFF1Index          CFF1CharStrings;
प्रकारव्याख्या Subrs<HBUINT16>    CFF1Subrs;

संरचना CFF1FDSelect : FDSelect {};

/* Encoding */
संरचना Encoding0 {
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) स्थिरांक
  {
    प्रतिज्ञा (glyph > 0);
    glyph--;
    जर (glyph < nCodes ())
    {
      परत (hb_codepoint_t)codes[glyph];
    }
    अन्यथा
      परत CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { परत codes.len; }
  HBUINT8 nCodes () स्थिरांक { परत codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

संरचना Encoding1_Range {
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  HBUINT8   first;
  HBUINT8   n_डावा;

  DEFINE_SIZE_STATIC (2);
};

संरचना Encoding1 {
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) स्थिरांक
  {
    /* TODO: Add cache like get_sid. */
    प्रतिज्ञा (glyph > 0);
    glyph--;
    साठी (अचिन्हित पूर्णांक i = 0; i < nRanges (); i++)
    {
      जर (glyph <= ranges[i].n_डावा)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	परत (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_डावा + 1);
    }
    परत CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { परत ranges.len; }
  HBUINT8 nRanges () स्थिरांक { परत ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

संरचना SuppEncoding {
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

संरचना CFF1SuppEncData {
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (supps.sanitize (c));
  }

  शून्य get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) स्थिरांक
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < nSups (); i++)
      जर (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { परत supps.len; }
  HBUINT8 nSups () स्थिरांक { परत supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

संरचना Encoding
{
  /* serialize a fullset Encoding */
  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक Encoding &src)
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  तार्किक serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  अचिन्हित पूर्णांक enc_मोजणी,
		  स्थिरांक hb_vector_t<code_pair_t>& code_ranges,
		  स्थिरांक hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (हे);
    Encoding *dest = c->extend_min (हे);
    जर (unlikely (!dest)) return_trace (असत्य);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    निवड (format) {
    स्थिती 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_मोजणी);
      जर (unlikely (!fmt0)) return_trace (असत्य);
      fmt0->nCodes () = enc_मोजणी;
      अचिन्हित पूर्णांक glyph = 0;
      साठी (अचिन्हित पूर्णांक i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	साठी (पूर्णांक left = (पूर्णांक)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	जर (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (असत्य);
      }
    }
    थांबा;

    स्थिती 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      जर (unlikely (!fmt1)) return_trace (असत्य);
      fmt1->nRanges () = code_ranges.length;
      साठी (अचिन्हित पूर्णांक i = 0; i < code_ranges.length; i++)
      {
	जर (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (असत्य);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_डावा = code_ranges[i].glyph;
      }
    }
    थांबा;

    }

    जर (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      जर (unlikely (!suppData)) return_trace (असत्य);
      suppData->nSups () = supp_codes.length;
      साठी (अचिन्हित पूर्णांक i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (सत्य);
  }

  माप_प्रकार get_size () स्थिरांक
  {
    माप_प्रकार size = min_size;
    निवड (table_format ())
    {
    स्थिती 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); थांबा;
    स्थिती 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); थांबा;
    }
    जर (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    परत size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) स्थिरांक
  {
    निवड (table_format ())
    {
    स्थिती 0: hb_barrier (); परत u.format0.get_code (glyph);
    स्थिती 1: hb_barrier (); परत u.format1.get_code (glyph);
    पूर्वनिर्धारित:परत 0;
    }
  }

  uint8_t table_format () स्थिरांक { परत format & 0x7F; }
  तार्किक  has_supplement () स्थिरांक { परत format & 0x80; }

  शून्य get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) स्थिरांक
  {
    codes.clear ();
    जर (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे)))
      return_trace (असत्य);
    hb_barrier ();

    निवड (table_format ())
    {
    स्थिती 0: hb_barrier (); जर (unlikely (!u.format0.sanitize (c))) { return_trace (असत्य); } थांबा;
    स्थिती 1: hb_barrier (); जर (unlikely (!u.format1.sanitize (c))) { return_trace (असत्य); } थांबा;
    पूर्वनिर्धारित:return_trace (असत्य);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  संरक्षित:
  स्थिरांक CFF1SuppEncData &suppEncData () स्थिरांक
  {
    निवड (table_format ())
    {
    स्थिती 0: hb_barrier (); परत StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    स्थिती 1: hb_barrier (); परत StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    पूर्वनिर्धारित:परत Null (CFF1SuppEncData);
    }
  }

  सार्वजनिक:
  HBUINT8	format;
  संघ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
संरचना Charset0
{
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक अंक_glyphs, अचिन्हित *अंक_charset_entries) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (अंक_charset_entries) *अंक_charset_entries = अंक_glyphs;
    return_trace (sids.sanitize (c, अंक_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, अचिन्हित अंक_glyphs) स्थिरांक
  {
    जर (unlikely (glyph >= अंक_glyphs)) परत 0;
    जर (unlikely (glyph == 0))
      परत 0;
    अन्यथा
      परत sids[glyph - 1];
  }

  शून्य collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    जर (!mapping->resize_dirty  (अंक_glyphs)) परत;
    साठी (hb_codepoint_t gid = 1; gid < अंक_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    जर (sid == 0)
      परत 0;

    साठी (अचिन्हित पूर्णांक glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      जर (sids[glyph-1] == sid)
	परत glyph;
    }
    परत 0;
  }

  स्थिर माप_प्रकार get_size (अचिन्हित पूर्णांक अंक_glyphs)
  {
    प्रतिज्ञा (अंक_glyphs > 0);
    परत UnsizedArrayOf<HBUINT16>::get_size (अंक_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

ढाँचा <typename TYPE>
संरचना Charset_Range {
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  HBUINT16  first;
  TYPE      n_डावा;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

ढाँचा <typename TYPE>
संरचना Charset1_2 {
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक अंक_glyphs, अचिन्हित *अंक_charset_entries) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    अंक_glyphs--;
    अचिन्हित i;
    साठी (i = 0; अंक_glyphs > 0; i++)
    {
      जर (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (अंक_glyphs >= ranges[i].n_डावा + 1))))
	return_trace (असत्य);
      अंक_glyphs -= (ranges[i].n_डावा + 1);
    }
    जर (अंक_charset_entries)
      *अंक_charset_entries = i;
    return_trace (सत्य);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, अचिन्हित अंक_glyphs,
			  code_pair_t *cache = nullptr) स्थिरांक
  {
    जर (unlikely (glyph >= अंक_glyphs)) परत 0;
    अचिन्हित i;
    hb_codepoint_t start_glyph;
    जर (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    अन्यथा
    {
      जर (unlikely (glyph == 0)) परत 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    साठी (;; i++)
    {
      अचिन्हित मोजणी = ranges[i].n_डावा;
      जर (glyph <= मोजणी)
      {
        जर (cache)
	  *cache = {i, start_glyph};
	परत ranges[i].first + glyph;
      }
      मोजणी++;
      start_glyph += मोजणी;
      glyph -= मोजणी;
    }

    परत 0;
  }

  शून्य collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    जर (!mapping->resize_dirty  (अंक_glyphs)) परत;

    hb_codepoint_t gid = 1;
    जर (gid >= अंक_glyphs)
      परत;

    साठी (अचिन्हित i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      अचिन्हित मोजणी = ranges[i].n_डावा + 1;
      अचिन्हित last = gid + मोजणी;
      साठी (अचिन्हित j = 0; j < मोजणी; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      जर (gid >= अंक_glyphs)
        थांबा;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    जर (sid == 0) परत 0;
    hb_codepoint_t  glyph = 1;
    साठी (अचिन्हित पूर्णांक i = 0;; i++)
    {
      जर (glyph >= अंक_glyphs)
	परत 0;
      जर ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_डावा))
	परत glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_डावा + 1);
    }

    परत 0;
  }

  माप_प्रकार get_size (अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    पूर्णांक glyph = (पूर्णांक) अंक_glyphs;
    अचिन्हित अंक_ranges = 0;

    प्रतिज्ञा (glyph > 0);
    glyph--;
    साठी (अचिन्हित पूर्णांक i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_डावा + 1);
      अंक_ranges++;
    }

    परत get_size_for_ranges (अंक_ranges);
  }

  स्थिर माप_प्रकार get_size_for_ranges (अचिन्हित पूर्णांक अंक_ranges)
  {
    परत UnsizedArrayOf<Charset_Range<TYPE> >::get_size (अंक_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

प्रकारव्याख्या Charset1_2<HBUINT8>     Charset1;
प्रकारव्याख्या Charset1_2<HBUINT16>    Charset2;
प्रकारव्याख्या Charset_Range<HBUINT8>  Charset1_Range;
प्रकारव्याख्या Charset_Range<HBUINT16> Charset2_Range;

संरचना Charset
{
  /* serialize a fullset Charset */
  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक Charset &src, अचिन्हित पूर्णांक अंक_glyphs)
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed ((स्थिरांक अक्षर *) &src, src.get_size (अंक_glyphs)));
  }

  /* serialize a subset Charset */
  तार्किक serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  अचिन्हित पूर्णांक अंक_glyphs,
		  स्थिरांक hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (हे);
    Charset *dest = c->extend_min (हे);
    जर (unlikely (!dest)) return_trace (असत्य);
    dest->format = format;
    निवड (format)
    {
    स्थिती 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (अंक_glyphs), असत्य);
      जर (unlikely (!fmt0)) return_trace (असत्य);
      अचिन्हित पूर्णांक glyph = 0;
      साठी (अचिन्हित पूर्णांक i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	साठी (पूर्णांक left = (पूर्णांक)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    थांबा;

    स्थिती 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), असत्य);
      जर (unlikely (!fmt1)) return_trace (असत्य);
      hb_codepoint_t all_glyphs = 0;
      साठी (अचिन्हित पूर्णांक i = 0; i < sid_ranges.length; i++)
      {
        स्वतः &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_डावा = _.glyph;
      }
      जर (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (असत्य);
    }
    थांबा;

    स्थिती 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), असत्य);
      जर (unlikely (!fmt2)) return_trace (असत्य);
      hb_codepoint_t all_glyphs = 0;
      साठी (अचिन्हित पूर्णांक i = 0; i < sid_ranges.length; i++)
      {
        स्वतः &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_डावा = _.glyph;
      }
      जर (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (असत्य);
    }
    थांबा;

    }
    return_trace (सत्य);
  }

  माप_प्रकार get_size (अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    निवड (format)
    {
    स्थिती 0: hb_barrier (); परत hb_unsigned_add_saturate (min_size, u.format0.get_size (अंक_glyphs));
    स्थिती 1: hb_barrier (); परत hb_unsigned_add_saturate (min_size, u.format1.get_size (अंक_glyphs));
    स्थिती 2: hb_barrier (); परत hb_unsigned_add_saturate (min_size, u.format2.get_size (अंक_glyphs));
    पूर्वनिर्धारित:परत 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, अचिन्हित पूर्णांक अंक_glyphs,
			  code_pair_t *cache = nullptr) स्थिरांक
  {
    निवड (format)
    {
    स्थिती 0: hb_barrier (); परत u.format0.get_sid (glyph, अंक_glyphs);
    स्थिती 1: hb_barrier (); परत u.format1.get_sid (glyph, अंक_glyphs, cache);
    स्थिती 2: hb_barrier (); परत u.format2.get_sid (glyph, अंक_glyphs, cache);
    पूर्वनिर्धारित:परत 0;
    }
  }

  शून्य collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    निवड (format)
    {
    स्थिती 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, अंक_glyphs); परत;
    स्थिती 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, अंक_glyphs); परत;
    स्थिती 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, अंक_glyphs); परत;
    पूर्वनिर्धारित:परत;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    निवड (format)
    {
    स्थिती 0: hb_barrier (); परत u.format0.get_glyph (sid, अंक_glyphs);
    स्थिती 1: hb_barrier (); परत u.format1.get_glyph (sid, अंक_glyphs);
    स्थिती 2: hb_barrier (); परत u.format2.get_glyph (sid, अंक_glyphs);
    पूर्वनिर्धारित:परत 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित *अंक_charset_entries) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे)))
      return_trace (असत्य);
    hb_barrier ();

    निवड (format)
    {
    स्थिती 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    स्थिती 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    स्थिती 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  HBUINT8       format;
  संघ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

संरचना CFF1StringIndex : CFF1Index
{
  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक CFF1StringIndex &strings,
		  स्थिरांक hb_vector_t<अचिन्हित> &sidmap)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely ((strings.मोजणी == 0) || (sidmap.length == 0)))
    {
      जर (unlikely (!c->extend_min (हे->मोजणी)))
	return_trace (असत्य);
      मोजणी = 0;
      return_trace (सत्य);
    }

    जर (unlikely (sidmap.in_error ())) return_trace (असत्य);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    जर (unlikely (bytesArray.in_error ())) return_trace (असत्य);

    तार्किक result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

संरचना cff1_शीर्ष_dict_interp_env_t : अंक_interp_env_t
{
  cff1_शीर्ष_dict_interp_env_t ()
    : अंक_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_शीर्ष_dict_interp_env_t (स्थिरांक hb_ubytes_t &bytes)
    : अंक_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  अचिन्हित पूर्णांक prev_offset;
  अचिन्हित पूर्णांक last_offset;
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
    साठी (अचिन्हित पूर्णांक i = 0; i < Val_मोजणी; i++)
      values[i] = CFF_UNDEF_SID;
  }

  अचिन्हित पूर्णांक& चालक[] (अचिन्हित पूर्णांक i)
  { प्रतिज्ञा (i < Val_मोजणी); परत values[i]; }

  अचिन्हित पूर्णांक चालक[] (अचिन्हित पूर्णांक i) स्थिरांक
  { प्रतिज्ञा (i < Val_मोजणी); परत values[i]; }

  स्थिर गणना name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    निवड (op) {
      पूर्वनिर्धारित: // can't happen - just make some compiler happy
      स्थिती OpCode_version:
	परत version;
      स्थिती OpCode_Notice:
	परत notice;
      स्थिती OpCode_Copyright:
	परत copyright;
      स्थिती OpCode_FullName:
	परत fullName;
      स्थिती OpCode_FamilyName:
	परत familyName;
      स्थिती OpCode_Weight:
	परत weight;
      स्थिती OpCode_PostScript:
	परत postscript;
      स्थिती OpCode_FontName:
	परत fontName;
      स्थिती OpCode_BaseFontName:
	परत baseFontName;
    }
  }

  अचिन्हित पूर्णांक  values[Val_मोजणी];
};

संरचना cff1_शीर्ष_dict_val_t : op_str_t
{
  अचिन्हित पूर्णांक  last_arg_offset;
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

  तार्किक is_CID () स्थिरांक
  { परत nameSIDs[name_dict_मूल्ये_t::registry] != CFF_UNDEF_SID; }

  name_dict_मूल्ये_t  nameSIDs;
  अचिन्हित पूर्णांक    ros_supplement_offset;
  अचिन्हित पूर्णांक    ros_supplement;
  अचिन्हित पूर्णांक    cid_मोजणी;

  पूर्णांक             EncodingOffset;
  पूर्णांक             CharsetOffset;
  पूर्णांक             FDSelectOffset;
  table_info_t    privateDictInfo;
};

संरचना cff1_शीर्ष_dict_opset_t : शीर्ष_dict_opset_t<cff1_शीर्ष_dict_val_t>
{
  स्थिर शून्य process_op (op_code_t op, cff1_शीर्ष_dict_interp_env_t& env, cff1_शीर्ष_dict_मूल्ये_t& dictval)
  {
    cff1_शीर्ष_dict_val_t  val {};

    निवड (op) {
      स्थिती OpCode_version:
      स्थिती OpCode_Notice:
      स्थिती OpCode_Copyright:
      स्थिती OpCode_FullName:
      स्थिती OpCode_FontName:
      स्थिती OpCode_FamilyName:
      स्थिती OpCode_Weight:
      स्थिती OpCode_PostScript:
      स्थिती OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_मूल्ये_t::name_op_to_index (op)] = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	थांबा;
      स्थिती OpCode_isFixedPitch:
      स्थिती OpCode_ItalicAngle:
      स्थिती OpCode_UnderlinePosition:
      स्थिती OpCode_UnderlineThickness:
      स्थिती OpCode_PaintType:
      स्थिती OpCode_CharstringType:
      स्थिती OpCode_UniqueID:
      स्थिती OpCode_StrokeWidth:
      स्थिती OpCode_SyntheticBase:
      स्थिती OpCode_CIDFontVersion:
      स्थिती OpCode_CIDFontRevision:
      स्थिती OpCode_CIDFontType:
      स्थिती OpCode_UIDBase:
      स्थिती OpCode_FontBBox:
      स्थिती OpCode_XUID:
      स्थिती OpCode_BaseFontBlend:
	env.clear_args ();
	थांबा;

      स्थिती OpCode_CIDCount:
	dictval.cid_मोजणी = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	थांबा;

      स्थिती OpCode_ROS:
	dictval.ros_supplement = env.arg_चळत.काढा_uint ();
	dictval.nameSIDs[name_dict_मूल्ये_t::ordering] = env.arg_चळत.काढा_uint ();
	dictval.nameSIDs[name_dict_मूल्ये_t::registry] = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	जर (unlikely (env.in_error ())) परत;
	प्रतिज्ञा (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	थांबा;

      स्थिती OpCode_Encoding:
	dictval.EncodingOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	जर (unlikely (dictval.EncodingOffset == 0)) परत;
	थांबा;

      स्थिती OpCode_charset:
	dictval.CharsetOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	जर (unlikely (dictval.CharsetOffset == 0)) परत;
	थांबा;

      स्थिती OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;

      स्थिती OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_चळत.काढा_int ();
	dictval.privateDictInfo.size = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	थांबा;

      पूर्वनिर्धारित:
	env.last_offset = env.str_ref.get_offset ();
	शीर्ष_dict_opset_t<cff1_शीर्ष_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	जर (!env.arg_चळत.is_empty ()) परत;
	थांबा;
    }

    जर (unlikely (env.in_error ())) परत;

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
  अचिन्हित पूर्णांक    fontName;
};

संरचना cff1_font_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, अंक_interp_env_t& env, cff1_font_dict_मूल्ये_t& dictval)
  {
    निवड (op) {
      स्थिती OpCode_FontName:
	dictval.fontName = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	थांबा;
      स्थिती OpCode_FontMatrix:
      स्थिती OpCode_PaintType:
	env.clear_args ();
	थांबा;
      स्थिती OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_चळत.काढा_uint ();
	dictval.privateDictInfo.size = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	थांबा;

      पूर्वनिर्धारित:
	dict_opset_t::process_op (op, env);
	जर (!env.arg_चळत.is_empty ()) परत;
	थांबा;
    }

    जर (unlikely (env.in_error ())) परत;

    dictval.add_op (op, env.str_ref);
  }
};

ढाँचा <typename VAL>
संरचना cff1_private_dict_मूल्ये_base_t : dict_मूल्ये_t<VAL>
{
  शून्य init ()
  {
    dict_मूल्ये_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  शून्य fini () { dict_मूल्ये_t<VAL>::fini (); }

  पूर्णांक                 subrsOffset;
  स्थिरांक CFF1Subrs    *localSubrs;
};

प्रकारव्याख्या cff1_private_dict_मूल्ये_base_t<op_str_t> cff1_private_dict_मूल्ये_subset_t;
प्रकारव्याख्या cff1_private_dict_मूल्ये_base_t<अंक_dict_val_t> cff1_private_dict_मूल्ये_t;

संरचना cff1_private_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, अंक_interp_env_t& env, cff1_private_dict_मूल्ये_t& dictval)
  {
    अंक_dict_val_t val;
    val.init ();

    निवड (op) {
      स्थिती Op_Code_Blue_मूल्ये:
      स्थिती OpCode_OtherBlues:
      स्थिती OpCode_FamilyBlues:
      स्थिती OpCode_FamilyOtherBlues:
      स्थिती OpCode_StemSnapH:
      स्थिती OpCode_StemSnapV:
      स्थिती OpCode_StdHW:
      स्थिती OpCode_StdVW:
      स्थिती OpCode_BlueScale:
      स्थिती OpCode_BlueShift:
      स्थिती OpCode_BlueFuzz:
      स्थिती OpCode_ForceBold:
      स्थिती OpCode_LanguageGroup:
      स्थिती OpCode_ExpansionFactor:
      स्थिती OpCode_initialRandomSeed:
      स्थिती OpCode_defaultWidthX:
      स्थिती OpCode_nominalWidthX:
	env.clear_args ();
	थांबा;
      स्थिती OpCode_Subrs:
	dictval.subrsOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;

      पूर्वनिर्धारित:
	dict_opset_t::process_op (op, env);
	जर (!env.arg_चळत.is_empty ()) परत;
	थांबा;
    }

    जर (unlikely (env.in_error ())) परत;

    dictval.add_op (op, env.str_ref, val);
  }
};

संरचना cff1_private_dict_opset_subset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, अंक_interp_env_t& env, cff1_private_dict_मूल्ये_subset_t& dictval)
  {
    निवड (op) {
      स्थिती Op_Code_Blue_मूल्ये:
      स्थिती OpCode_OtherBlues:
      स्थिती OpCode_FamilyBlues:
      स्थिती OpCode_FamilyOtherBlues:
      स्थिती OpCode_StemSnapH:
      स्थिती OpCode_StemSnapV:
      स्थिती OpCode_StdHW:
      स्थिती OpCode_StdVW:
      स्थिती OpCode_BlueScale:
      स्थिती OpCode_BlueShift:
      स्थिती OpCode_BlueFuzz:
      स्थिती OpCode_ForceBold:
      स्थिती OpCode_LanguageGroup:
      स्थिती OpCode_ExpansionFactor:
      स्थिती OpCode_initialRandomSeed:
      स्थिती OpCode_defaultWidthX:
      स्थिती OpCode_nominalWidthX:
	env.clear_args ();
	थांबा;

      स्थिती OpCode_Subrs:
	dictval.subrsOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;

      पूर्वनिर्धारित:
	dict_opset_t::process_op (op, env);
	जर (!env.arg_चळत.is_empty ()) परत;
	थांबा;
    }

    जर (unlikely (env.in_error ())) परत;

    dictval.add_op (op, env.str_ref);
  }
};

प्रकारव्याख्या dict_interpreter_t<cff1_शीर्ष_dict_opset_t, cff1_शीर्ष_dict_मूल्ये_t, cff1_शीर्ष_dict_interp_env_t> cff1_शीर्ष_dict_interpreter_t;
प्रकारव्याख्या dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_मूल्ये_t> cff1_font_dict_interpreter_t;

प्रकारव्याख्या CFF1Index CFF1NameIndex;
प्रकारव्याख्या CFF1Index CFF1_शीर्ष_Dict_Index;

संरचना cff1_font_dict_मूल्ये_mod_t
{
  cff1_font_dict_मूल्ये_mod_t() { init (); }

  शून्य init () { init ( &Null (cff1_font_dict_मूल्ये_t), CFF_UNDEF_SID ); }

  शून्य init (स्थिरांक cff1_font_dict_मूल्ये_t *base_,
	     अचिन्हित पूर्णांक fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  अचिन्हित get_मोजणी () स्थिरांक { परत base->get_मोजणी (); }

  स्थिरांक op_str_t &चालक [] (अचिन्हित पूर्णांक i) स्थिरांक { परत (*base)[i]; }

  स्थिरांक cff1_font_dict_मूल्ये_t    *base;
  table_info_t		   privateDictInfo;
  अचिन्हित पूर्णांक		fontName;
};

संरचना CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  ढाँचा <typename ITER, typename OP_SERIALIZER>
  तार्किक serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { परत FDArray<HBUINT16>::serialize<cff1_font_dict_मूल्ये_mod_t, cff1_font_dict_मूल्ये_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

नामाकाश OT {

using नामाकाश CFF;

संरचना cff1
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  ढाँचा <typename PRIVOPSET, typename PRIVDICTVAL>
  संरचना accelerator_templ_t
  {
    स्थिर constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      जर (!face) परत;

      शीर्ष_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      हे->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (हे->blob);
      sc.start_processing ();

      स्थिरांक OT::cff1 *cff = हे->blob->ढाँचा as<OT::cff1> ();

      जर (cff == &Null (OT::cff1))
        जा fail;

      nameIndex = &cff->nameIndex (cff);
      जर ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        जा fail;
      hb_barrier ();

      शीर्ष_Dict_Index = &StructAtOffsetOrNull<CFF1_शीर्ष_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      जर (शीर्ष_Dict_Index == &Null (CFF1_शीर्ष_Dict_Index) || (शीर्ष_Dict_Index->मोजणी == 0))
        जा fail;
      hb_barrier ();

      { /* parse top dict */
	स्थिरांक hb_ubytes_t शीर्ष_Dict_Str = (*शीर्ष_Dict_Index)[0];
	जर (unlikely (!शीर्ष_Dict_Str.sanitize (&sc)))   जा fail;
	hb_barrier ();
	cff1_शीर्ष_dict_interp_env_t env (शीर्ष_Dict_Str);
	cff1_शीर्ष_dict_interpreter_t शीर्ष_interp (env);
	जर (unlikely (!शीर्ष_interp.interpret (शीर्ष_Dict_2)))   जा fail;
      }

      जर (is_predef_charset ())
	charset = &Null (Charset);
      अन्यथा
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, शीर्ष_Dict_2.CharsetOffset, sc, &अंक_charset_entries);
	जर (unlikely (charset == &Null (Charset)))   जा fail;
      }

      fd_मोजणी = 1;
      जर (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, शीर्ष_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, शीर्ष_Dict_2.FDSelectOffset, sc, fdArray->मोजणी);
	जर (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  जा fail;

	fd_मोजणी = fdArray->मोजणी;
      }
      अन्यथा
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      जर (is_CID ())
      {
	जर (unlikely (charset == &Null (Charset)))   जा fail;
      }
      अन्यथा
      {
	जर (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, शीर्ष_Dict_2.EncodingOffset, sc);
	  जर (unlikely (encoding == &Null (Encoding)))   जा fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (शीर्ष_Dict_Index, शीर्ष_Dict_Index->get_size (), sc);
      जर (stringIndex == &Null (CFF1StringIndex))
        जा fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, शीर्ष_Dict_2.charStringsOffset, sc);
      जर (charStrings == &Null (CFF1CharStrings))
        जा fail;

      अंक_glyphs = charStrings->मोजणी;
      जर (अंक_glyphs != sc.get_अंक_glyphs ())
        जा fail;

      जर (unlikely (!privateDicts.resize (fd_मोजणी)))
        जा fail;
      साठी (अचिन्हित पूर्णांक i = 0; i < fd_मोजणी; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      जर (is_CID ())
      {
	साठी (अचिन्हित पूर्णांक i = 0; i < fd_मोजणी; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  जर (unlikely (!fontDictStr.sanitize (&sc)))   जा fail;
	  hb_barrier ();
	  cff1_font_dict_मूल्ये_t *font;
	  cff1_शीर्ष_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  जर (unlikely (fontDicts.in_error ()))   जा fail;

	  font->init ();
	  जर (unlikely (!font_interp.interpret (*font)))   जा fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  स्थिरांक hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  जर (unlikely (font->privateDictInfo.size &&
			privDictStr == (स्थिरांक अचिन्हित अक्षर *) &Null (UnsizedByteStr))) जा fail;
	  अंक_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  जर (unlikely (!priv_interp.interpret (*priv)))   जा fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      अन्यथा  /* non-CID */
      {
	cff1_शीर्ष_dict_मूल्ये_t *font = &शीर्ष_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	स्थिरांक hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	जर (font->privateDictInfo.size &&
	    unlikely (privDictStr == (स्थिरांक अचिन्हित अक्षर *) &Null (UnsizedByteStr))) जा fail;
	अंक_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	जर (unlikely (!priv_interp.interpret (*priv)))   जा fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      परत;

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

    hb_blob_t *get_blob () स्थिरांक { परत blob; }

    तार्किक is_valid () स्थिरांक { परत blob; }
    तार्किक   is_CID () स्थिरांक { परत शीर्ष_Dict_2.is_CID (); }

    तार्किक is_predef_charset () स्थिरांक { परत शीर्ष_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    अचिन्हित पूर्णांक std_code_to_glyph (hb_codepoint_t code) स्थिरांक
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      जर (unlikely (sid == CFF_UNDEF_SID))
	परत 0;

      जर (charset != &Null (Charset))
	परत charset->get_glyph (sid, अंक_glyphs);
      अन्यथा जर ((शीर्ष_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) परत sid;
      परत 0;
    }

    तार्किक is_predef_encoding () स्थिरांक { परत शीर्ष_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) स्थिरांक
    {
      जर (encoding != &Null (Encoding))
	परत encoding->get_code (glyph);
      अन्यथा
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	जर (sid == 0) परत 0;
	hb_codepoint_t code = 0;
	निवड (शीर्ष_Dict_2.EncodingOffset)
	{
	स्थिती StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  थांबा;
	स्थिती ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  थांबा;
	पूर्वनिर्धारित:
	  थांबा;
	}
	परत code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () स्थिरांक
    {
      जर (charset != &Null (Charset))
      {
	स्वतः *mapping = (glyph_to_sid_map_t *) hb_malloc (आकार (glyph_to_sid_map_t));
	जर (unlikely (!mapping)) परत nullptr;
	mapping = नवीन (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, अंक_glyphs);
	परत mapping;
      }
      अन्यथा
	परत nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) स्थिरांक
    {
      जर (charset != &Null (Charset))
	परत charset->get_sid (glyph, अंक_glyphs, cache);
      अन्यथा
      {
	hb_codepoint_t sid = 0;
	निवड (शीर्ष_Dict_2.CharsetOffset)
	{
	  स्थिती ISOAdobeCharset:
	    जर (glyph <= 228 /*zcaron*/) sid = glyph;
	    थांबा;
	  स्थिती ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    थांबा;
	  स्थिती ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    थांबा;
	  पूर्वनिर्धारित:
	    थांबा;
	}
	परत sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) स्थिरांक
    {
      जर (charset != &Null (Charset))
	परत charset->get_glyph (sid, अंक_glyphs);
      अन्यथा
      {
	hb_codepoint_t glyph = 0;
	निवड (शीर्ष_Dict_2.CharsetOffset)
	{
	  स्थिती ISOAdobeCharset:
	    जर (sid <= 228 /*zcaron*/) glyph = sid;
	    थांबा;
	  स्थिती ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    थांबा;
	  स्थिती ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    थांबा;
	  पूर्वनिर्धारित:
	    थांबा;
	}
	परत glyph;
      }
    }

    संरक्षित:
    hb_sanitize_context_t   sc;

    सार्वजनिक:
    hb_blob_t               *blob = nullptr;
    स्थिरांक Encoding	    *encoding = nullptr;
    स्थिरांक Charset	    *charset = nullptr;
    स्थिरांक CFF1NameIndex     *nameIndex = nullptr;
    स्थिरांक CFF1_शीर्ष_Dict_Index  *शीर्ष_Dict_Index = nullptr;
    स्थिरांक CFF1StringIndex   *stringIndex = nullptr;
    स्थिरांक CFF1Subrs	    *globalSubrs = nullptr;
    स्थिरांक CFF1CharStrings   *charStrings = nullptr;
    स्थिरांक CFF1FDArray       *fdArray = nullptr;
    स्थिरांक CFF1FDSelect      *fdSelect = nullptr;
    अचिन्हित पूर्णांक	     fd_मोजणी = 0;

    cff1_शीर्ष_dict_मूल्ये_t   शीर्ष_Dict_2;
    hb_vector_t<cff1_font_dict_मूल्ये_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    अचिन्हित पूर्णांक	     अंक_glyphs = 0;
    अचिन्हित पूर्णांक	     अंक_charset_entries = 0;
  };

  संरचना accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_मूल्ये_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      जर (!is_valid ()) परत;
      जर (is_CID ()) परत;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      जर (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    तार्किक get_glyph_name (hb_codepoint_t glyph,
			 अक्षर *buf, अचिन्हित पूर्णांक buf_len) स्थिरांक
    {
      जर (unlikely (glyph >= अंक_glyphs)) परत असत्य;
      जर (unlikely (!is_valid ())) परत असत्य;
      जर (is_CID()) परत असत्य;
      जर (unlikely (!buf_len)) परत सत्य;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      स्थिरांक अक्षर *str;
      माप_प्रकार str_len;
      जर (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      अन्यथा
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (स्थिरांक अक्षर *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      जर (!str_len) परत असत्य;
      अचिन्हित पूर्णांक len = hb_min (buf_len - 1, str_len);
      म_नकलन (buf, (स्थिरांक अक्षर*)str, len);
      buf[len] = '\0';
      परत सत्य;
    }

    तार्किक get_glyph_from_name (स्थिरांक अक्षर *name, पूर्णांक len,
			      hb_codepoint_t *glyph) स्थिरांक
    {
      जर (unlikely (!is_valid ())) परत असत्य;
      जर (is_CID()) परत असत्य;
      जर (len < 0) len = म_माप (name);
      जर (unlikely (!len)) परत असत्य;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      जर (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, आकार (hb_sorted_vector_t<gname_t>));
	जर (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  साठी (hb_codepoint_t gid = 0; gid < अंक_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    जर (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    अन्यथा
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((स्थिरांक अक्षर*) ustr.arrayZ, ustr.length);
	    }
	    जर (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->क्विक ();
	}
	जर (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  जर (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  जा retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      स्थिरांक gname_t *gname = names ? names->द्वा_खोज (key) : nullptr;
      जर (!gname) परत असत्य;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      जर (!gid && gname->sid) परत असत्य;
      *glyph = gid;
      परत सत्य;
    }

    HB_INTERNAL तार्किक get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) स्थिरांक;
    HB_INTERNAL तार्किक get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) स्थिरांक;

    खाजगी:
    संरचना gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      स्थिर पूर्णांक cmp (स्थिरांक शून्य *a_, स्थिरांक शून्य *b_)
      {
	स्थिरांक gname_t *a = (स्थिरांक gname_t *)a_;
	स्थिरांक gname_t *b = (स्थिरांक gname_t *)b_;
	अचिन्हित minlen = hb_min (a->name.length, b->name.length);
	पूर्णांक ret = म_भेदन (a->name.arrayZ, b->name.arrayZ, minlen);
	जर (ret) परत ret;
	परत a->name.length - b->name.length;
      }

      पूर्णांक cmp (स्थिरांक gname_t &a) स्थिरांक { परत cmp (&a, हे); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    प्रकारव्याख्या accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_मूल्ये_t> SUPER;
  };

  संरचना accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_मूल्ये_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      जर (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL तार्किक subset (hb_subset_context_t *c) स्थिरांक;
    HB_INTERNAL तार्किक serialize (hb_serialize_context_t *c,
				संरचना cff1_subset_plan &plan) स्थिरांक;
    HB_INTERNAL तार्किक get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) स्थिरांक;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    प्रकारव्याख्या accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_मूल्ये_subset_t> SUPER;
  };

  संरक्षित:
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

  शून्य depend (hb_depend_data_builder_t *builder) स्थिरांक
  {
    जर (!is_valid ()) परत;
    साठी (hb_codepoint_t gid = 0; gid < अंक_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      जर (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
