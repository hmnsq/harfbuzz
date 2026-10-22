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

#अगर_अघोषित HB_OT_CFF1_TABLE_HH
#घोषणा HB_OT_CFF1_TABLE_HH

#समावेश "hb-ot-cff-common.hh"
#समावेश "hb-depend-data.hh"
#समावेश "hb-subset-cff-common.hh"
#समावेश "hb-draw.hh"
#समावेश "hb-paint.hh"

#घोषणा HB_STRING_ARRAY_NAME cff1_std_strings
#घोषणा HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#समावेश "hb-string-array.hh"
#अघोषित HB_STRING_ARRAY_LIST
#अघोषित HB_STRING_ARRAY_NAME

namespace CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#घोषणा HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#घोषणा CFF_UNDEF_SID   CFF_UNDEF_CODE

क्रमागत EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
क्रमागत CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

प्रकार CFF1_सूचकांक          CFF1CharStrings;
प्रकार Subrs<HBUINT16>    CFF1Subrs;

काष्ठा CFF1FDSelect : FDSelect {};

/* Encoding */
काष्ठा Encoding0 {
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) स्थिर
  {
    निश्चित (glyph > 0);
    glyph--;
    अगर (glyph < nCodes ())
    {
      वापस (hb_codepoint_t)codes[glyph];
    }
    अन्यथा
      वापस CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { वापस codes.len; }
  HBUINT8 nCodes () स्थिर { वापस codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_आकार_ARRAY_SIZED (1, codes);
};

काष्ठा Encoding1_Range {
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  HBUINT8   first;
  HBUINT8   n_बायाँ;

  DEFINE_आकार_STATIC (2);
};

काष्ठा Encoding1 {
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) स्थिर
  {
    /* TODO: Add cache like get_sid. */
    निश्चित (glyph > 0);
    glyph--;
    क्रम (अचिन्हित पूर्णांक i = 0; i < nRanges (); i++)
    {
      अगर (glyph <= ranges[i].n_बायाँ)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	वापस (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_बायाँ + 1);
    }
    वापस CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { वापस ranges.len; }
  HBUINT8 nRanges () स्थिर { वापस ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_आकार_ARRAY_SIZED (1, ranges);
};

काष्ठा SuppEncoding {
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_आकार_STATIC (3);
};

काष्ठा CFF1_Supp_Enc_तथ्य {
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (supps.sanitize (c));
  }

  व्योम get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) स्थिर
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < nSups (); i++)
      अगर (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { वापस supps.len; }
  HBUINT8 nSups () स्थिर { वापस supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_आकार_ARRAY_SIZED (1, supps);
};

काष्ठा Encoding
{
  /* serialize a fullset Encoding */
  bool serialize (hb_serialize_context_t *c, स्थिर Encoding &src)
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  bool serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  अचिन्हित पूर्णांक enc_गिनती,
		  स्थिर hb_vector_t<code_pair_t>& code_ranges,
		  स्थिर hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (यह);
    Encoding *dest = c->extend_min (यह);
    अगर (unlikely (!dest)) return_trace (false);
    dest->format = format | ((supp_codes.लंबाई > 0) ? 0x80 : 0);
    चयन (format) {
    हाल 0:
    {
      Encoding0 *fmt0 = c->allocate_आकार<Encoding0> (Encoding0::min_आकार + HBUINT8::static_आकार * enc_गिनती);
      अगर (unlikely (!fmt0)) return_trace (false);
      fmt0->nCodes () = enc_गिनती;
      अचिन्हित पूर्णांक glyph = 0;
      क्रम (अचिन्हित पूर्णांक i = 0; i < code_ranges.लंबाई; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	क्रम (पूर्णांक बाँए = (पूर्णांक)code_ranges[i].glyph; बाँए >= 0; बाँए--)
	  fmt0->codes[glyph++] = code++;
	अगर (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (false);
      }
    }
    अवरोध;

    हाल 1:
    {
      Encoding1 *fmt1 = c->allocate_आकार<Encoding1> (Encoding1::min_आकार + Encoding1_Range::static_आकार * code_ranges.लंबाई);
      अगर (unlikely (!fmt1)) return_trace (false);
      fmt1->nRanges () = code_ranges.लंबाई;
      क्रम (अचिन्हित पूर्णांक i = 0; i < code_ranges.लंबाई; i++)
      {
	अगर (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (false);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_बायाँ = code_ranges[i].glyph;
      }
    }
    अवरोध;

    }

    अगर (supp_codes.लंबाई)
    {
      CFF1_Supp_Enc_तथ्य *supp_तथ्य = c->allocate_आकार<CFF1_Supp_Enc_तथ्य> (CFF1_Supp_Enc_तथ्य::min_आकार + SuppEncoding::static_आकार * supp_codes.लंबाई);
      अगर (unlikely (!supp_तथ्य)) return_trace (false);
      supp_तथ्य->nSups () = supp_codes.लंबाई;
      क्रम (अचिन्हित पूर्णांक i = 0; i < supp_codes.लंबाई; i++)
      {
	supp_तथ्य->supps[i].code = supp_codes[i].code;
	supp_तथ्य->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (true);
  }

  माप_प्रकार get_आकार () स्थिर
  {
    माप_प्रकार आकार = min_आकार;
    चयन (table_format ())
    {
    हाल 0: hb_barrier (); आकार = hb_unsigned_add_saturate (आकार, u.format0.get_आकार ()); अवरोध;
    हाल 1: hb_barrier (); आकार = hb_unsigned_add_saturate (आकार, u.format1.get_आकार ()); अवरोध;
    }
    अगर (has_supplement ())
      आकार = hb_unsigned_add_saturate (आकार, supp_Enc_तथ्य ().get_आकार ());
    वापस आकार;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) स्थिर
  {
    चयन (table_format ())
    {
    हाल 0: hb_barrier (); वापस u.format0.get_code (glyph);
    हाल 1: hb_barrier (); वापस u.format1.get_code (glyph);
    शेष:वापस 0;
    }
  }

  uint8_t table_format () स्थिर { वापस format & 0x7F; }
  bool  has_supplement () स्थिर { वापस format & 0x80; }

  व्योम get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) स्थिर
  {
    codes.सफा ();
    अगर (has_supplement ())
      supp_Enc_तथ्य().get_codes (sid, codes);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह)))
      return_trace (false);
    hb_barrier ();

    चयन (table_format ())
    {
    हाल 0: hb_barrier (); अगर (unlikely (!u.format0.sanitize (c))) { return_trace (false); } अवरोध;
    हाल 1: hb_barrier (); अगर (unlikely (!u.format1.sanitize (c))) { return_trace (false); } अवरोध;
    शेष:return_trace (false);
    }
    return_trace (likely (!has_supplement () || supp_Enc_तथ्य ().sanitize (c)));
  }

  रक्षित:
  स्थिर CFF1_Supp_Enc_तथ्य &supp_Enc_तथ्य () स्थिर
  {
    चयन (table_format ())
    {
    हाल 0: hb_barrier (); वापस StructAfter<CFF1_Supp_Enc_तथ्य> (u.format0.codes[u.format0.nCodes ()-1]);
    हाल 1: hb_barrier (); वापस StructAfter<CFF1_Supp_Enc_तथ्य> (u.format1.ranges[u.format1.nRanges ()-1]);
    शेष:वापस Null (CFF1_Supp_Enc_तथ्य);
    }
  }

  खुला:
  HBUINT8	format;
  जोड़ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_आकार_MIN (1);
};

/* Charset */
काष्ठा Charset0
{
  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक अंक_glyphs, अचिन्हित *अंक_charset_entries) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (अंक_charset_entries) *अंक_charset_entries = अंक_glyphs;
    return_trace (sids.sanitize (c, अंक_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, अचिन्हित अंक_glyphs) स्थिर
  {
    अगर (unlikely (glyph >= अंक_glyphs)) वापस 0;
    अगर (unlikely (glyph == 0))
      वापस 0;
    अन्यथा
      वापस sids[glyph - 1];
  }

  व्योम collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    अगर (!mapping->resize_dirty  (अंक_glyphs)) वापस;
    क्रम (hb_codepoint_t gid = 1; gid < अंक_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    अगर (sid == 0)
      वापस 0;

    क्रम (अचिन्हित पूर्णांक glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      अगर (sids[glyph-1] == sid)
	वापस glyph;
    }
    वापस 0;
  }

  अटल माप_प्रकार get_आकार (अचिन्हित पूर्णांक अंक_glyphs)
  {
    निश्चित (अंक_glyphs > 0);
    वापस UnsizedArrayOf<HBUINT16>::get_आकार (अंक_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_आकार_ARRAY(0, sids);
};

ढाँचा <typename TYPE>
काष्ठा Charset_Range {
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  HBUINT16  first;
  TYPE      n_बायाँ;

  DEFINE_आकार_STATIC (HBUINT16::static_आकार + TYPE::static_आकार);
};

ढाँचा <typename TYPE>
काष्ठा Charset1_2 {
  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक अंक_glyphs, अचिन्हित *अंक_charset_entries) स्थिर
  {
    TRACE_SANITIZE (यह);
    अंक_glyphs--;
    अचिन्हित i;
    क्रम (i = 0; अंक_glyphs > 0; i++)
    {
      अगर (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (अंक_glyphs >= ranges[i].n_बायाँ + 1))))
	return_trace (false);
      अंक_glyphs -= (ranges[i].n_बायाँ + 1);
    }
    अगर (अंक_charset_entries)
      *अंक_charset_entries = i;
    return_trace (true);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, अचिन्हित अंक_glyphs,
			  code_pair_t *cache = nullptr) स्थिर
  {
    अगर (unlikely (glyph >= अंक_glyphs)) वापस 0;
    अचिन्हित i;
    hb_codepoint_t start_glyph;
    अगर (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    अन्यथा
    {
      अगर (unlikely (glyph == 0)) वापस 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    क्रम (;; i++)
    {
      अचिन्हित गिनती = ranges[i].n_बायाँ;
      अगर (glyph <= गिनती)
      {
        अगर (cache)
	  *cache = {i, start_glyph};
	वापस ranges[i].first + glyph;
      }
      गिनती++;
      start_glyph += गिनती;
      glyph -= गिनती;
    }

    वापस 0;
  }

  व्योम collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    अगर (!mapping->resize_dirty  (अंक_glyphs)) वापस;

    hb_codepoint_t gid = 1;
    अगर (gid >= अंक_glyphs)
      वापस;

    क्रम (अचिन्हित i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      अचिन्हित गिनती = ranges[i].n_बायाँ + 1;
      अचिन्हित last = gid + गिनती;
      क्रम (अचिन्हित j = 0; j < गिनती; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      अगर (gid >= अंक_glyphs)
        अवरोध;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    अगर (sid == 0) वापस 0;
    hb_codepoint_t  glyph = 1;
    क्रम (अचिन्हित पूर्णांक i = 0;; i++)
    {
      अगर (glyph >= अंक_glyphs)
	वापस 0;
      अगर ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_बायाँ))
	वापस glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_बायाँ + 1);
    }

    वापस 0;
  }

  माप_प्रकार get_आकार (अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    पूर्णांक glyph = (पूर्णांक) अंक_glyphs;
    अचिन्हित अंक_ranges = 0;

    निश्चित (glyph > 0);
    glyph--;
    क्रम (अचिन्हित पूर्णांक i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_बायाँ + 1);
      अंक_ranges++;
    }

    वापस get_आकार_for_ranges (अंक_ranges);
  }

  अटल माप_प्रकार get_आकार_for_ranges (अचिन्हित पूर्णांक अंक_ranges)
  {
    वापस UnsizedArrayOf<Charset_Range<TYPE> >::get_आकार (अंक_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_आकार_ARRAY (0, ranges);
};

प्रकार Charset1_2<HBUINT8>     Charset1;
प्रकार Charset1_2<HBUINT16>    Charset2;
प्रकार Charset_Range<HBUINT8>  Charset1_Range;
प्रकार Charset_Range<HBUINT16> Charset2_Range;

काष्ठा Charset
{
  /* serialize a fullset Charset */
  bool serialize (hb_serialize_context_t *c, स्थिर Charset &src, अचिन्हित पूर्णांक अंक_glyphs)
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed ((स्थिर अक्षर *) &src, src.get_आकार (अंक_glyphs)));
  }

  /* serialize a subset Charset */
  bool serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  अचिन्हित पूर्णांक अंक_glyphs,
		  स्थिर hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (यह);
    Charset *dest = c->extend_min (यह);
    अगर (unlikely (!dest)) return_trace (false);
    dest->format = format;
    चयन (format)
    {
    हाल 0:
    {
      Charset0 *fmt0 = c->allocate_आकार<Charset0> (Charset0::get_आकार (अंक_glyphs), false);
      अगर (unlikely (!fmt0)) return_trace (false);
      अचिन्हित पूर्णांक glyph = 0;
      क्रम (अचिन्हित पूर्णांक i = 0; i < sid_ranges.लंबाई; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	क्रम (पूर्णांक बाँए = (पूर्णांक)sid_ranges.arrayZ[i].glyph; बाँए >= 0; बाँए--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    अवरोध;

    हाल 1:
    {
      Charset1 *fmt1 = c->allocate_आकार<Charset1> (Charset1::get_आकार_for_ranges (sid_ranges.लंबाई), false);
      अगर (unlikely (!fmt1)) return_trace (false);
      hb_codepoint_t all_glyphs = 0;
      क्रम (अचिन्हित पूर्णांक i = 0; i < sid_ranges.लंबाई; i++)
      {
        स्वतः &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_बायाँ = _.glyph;
      }
      अगर (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (false);
    }
    अवरोध;

    हाल 2:
    {
      Charset2 *fmt2 = c->allocate_आकार<Charset2> (Charset2::get_आकार_for_ranges (sid_ranges.लंबाई), false);
      अगर (unlikely (!fmt2)) return_trace (false);
      hb_codepoint_t all_glyphs = 0;
      क्रम (अचिन्हित पूर्णांक i = 0; i < sid_ranges.लंबाई; i++)
      {
        स्वतः &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_बायाँ = _.glyph;
      }
      अगर (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (false);
    }
    अवरोध;

    }
    return_trace (true);
  }

  माप_प्रकार get_आकार (अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    चयन (format)
    {
    हाल 0: hb_barrier (); वापस hb_unsigned_add_saturate (min_आकार, u.format0.get_आकार (अंक_glyphs));
    हाल 1: hb_barrier (); वापस hb_unsigned_add_saturate (min_आकार, u.format1.get_आकार (अंक_glyphs));
    हाल 2: hb_barrier (); वापस hb_unsigned_add_saturate (min_आकार, u.format2.get_आकार (अंक_glyphs));
    शेष:वापस 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, अचिन्हित पूर्णांक अंक_glyphs,
			  code_pair_t *cache = nullptr) स्थिर
  {
    चयन (format)
    {
    हाल 0: hb_barrier (); वापस u.format0.get_sid (glyph, अंक_glyphs);
    हाल 1: hb_barrier (); वापस u.format1.get_sid (glyph, अंक_glyphs, cache);
    हाल 2: hb_barrier (); वापस u.format2.get_sid (glyph, अंक_glyphs, cache);
    शेष:वापस 0;
    }
  }

  व्योम collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    चयन (format)
    {
    हाल 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, अंक_glyphs); वापस;
    हाल 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, अंक_glyphs); वापस;
    हाल 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, अंक_glyphs); वापस;
    शेष:वापस;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    चयन (format)
    {
    हाल 0: hb_barrier (); वापस u.format0.get_glyph (sid, अंक_glyphs);
    हाल 1: hb_barrier (); वापस u.format1.get_glyph (sid, अंक_glyphs);
    हाल 2: hb_barrier (); वापस u.format2.get_glyph (sid, अंक_glyphs);
    शेष:वापस 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित *अंक_charset_entries) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह)))
      return_trace (false);
    hb_barrier ();

    चयन (format)
    {
    हाल 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_अंक_glyphs (), अंक_charset_entries));
    शेष:return_trace (false);
    }
  }

  HBUINT8       format;
  जोड़ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_आकार_MIN (1);
};

काष्ठा CFF1_String_सूचकांक : CFF1_सूचकांक
{
  bool serialize (hb_serialize_context_t *c, स्थिर CFF1_String_सूचकांक &strings,
		  स्थिर hb_vector_t<अचिन्हित> &sidmap)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely ((strings.गिनती == 0) || (sidmap.लंबाई == 0)))
    {
      अगर (unlikely (!c->extend_min (यह->गिनती)))
	return_trace (false);
      गिनती = 0;
      return_trace (true);
    }

    अगर (unlikely (sidmap.in_error ())) return_trace (false);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    अगर (unlikely (bytesArray.in_error ())) return_trace (false);

    bool परिणाम = CFF1_सूचकांक::serialize (c, bytesArray);
    return_trace (परिणाम);
  }
};

काष्ठा cff1_शीर्ष_dict_interp_env_t : अंक_interp_env_t
{
  cff1_शीर्ष_dict_interp_env_t ()
    : अंक_interp_env_t(), पूर्व_offset(0), last_offset(0) {}
  cff1_शीर्ष_dict_interp_env_t (स्थिर hb_ubytes_t &bytes)
    : अंक_interp_env_t(bytes), पूर्व_offset(0), last_offset(0) {}

  अचिन्हित पूर्णांक पूर्व_offset;
  अचिन्हित पूर्णांक last_offset;
};

काष्ठा name_dict_मूल्यावली_t
{
  क्रमागत name_dict_val_सूचकांक_t
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

      Val_गिनती
  };

  व्योम init ()
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < Val_गिनती; i++)
      values[i] = CFF_UNDEF_SID;
  }

  अचिन्हित पूर्णांक& चालक[] (अचिन्हित पूर्णांक i)
  { निश्चित (i < Val_गिनती); वापस values[i]; }

  अचिन्हित पूर्णांक चालक[] (अचिन्हित पूर्णांक i) स्थिर
  { निश्चित (i < Val_गिनती); वापस values[i]; }

  अटल क्रमागत name_dict_val_सूचकांक_t name_op_to_सूचकांक (op_code_t op)
  {
    चयन (op) {
      शेष: // can't happen - just make some compiler happy
      हाल OpCode_version:
	वापस version;
      हाल OpCode_Notice:
	वापस notice;
      हाल OpCode_Copyright:
	वापस copyright;
      हाल OpCode_FullName:
	वापस fullName;
      हाल OpCode_FamilyName:
	वापस familyName;
      हाल OpCode_Weight:
	वापस weight;
      हाल OpCode_PostScript:
	वापस postscript;
      हाल OpCode_FontName:
	वापस fontName;
      हाल OpCode_BaseFontName:
	वापस baseFontName;
    }
  }

  अचिन्हित पूर्णांक  values[Val_गिनती];
};

काष्ठा cff1_शीर्ष_dict_val_t : op_str_t
{
  अचिन्हित पूर्णांक  last_arg_offset;
};

काष्ठा cff1_शीर्ष_dict_मूल्यावली_t : शीर्ष_dict_मूल्यावली_t<cff1_शीर्ष_dict_val_t>
{
  व्योम init ()
  {
    शीर्ष_dict_मूल्यावली_t<cff1_शीर्ष_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_गिनती = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  व्योम fini () { शीर्ष_dict_मूल्यावली_t<cff1_शीर्ष_dict_val_t>::fini (); }

  bool is_CID () स्थिर
  { वापस nameSIDs[name_dict_मूल्यावली_t::registry] != CFF_UNDEF_SID; }

  name_dict_मूल्यावली_t  nameSIDs;
  अचिन्हित पूर्णांक    ros_supplement_offset;
  अचिन्हित पूर्णांक    ros_supplement;
  अचिन्हित पूर्णांक    cid_गिनती;

  पूर्णांक             EncodingOffset;
  पूर्णांक             CharsetOffset;
  पूर्णांक             FDSelectOffset;
  table_info_t    privateDictInfo;
};

काष्ठा cff1_शीर्ष_dict_opset_t : शीर्ष_dict_opset_t<cff1_शीर्ष_dict_val_t>
{
  अटल व्योम process_op (op_code_t op, cff1_शीर्ष_dict_interp_env_t& env, cff1_शीर्ष_dict_मूल्यावली_t& dictval)
  {
    cff1_शीर्ष_dict_val_t  val {};

    चयन (op) {
      हाल OpCode_version:
      हाल OpCode_Notice:
      हाल OpCode_Copyright:
      हाल OpCode_FullName:
      हाल OpCode_FontName:
      हाल OpCode_FamilyName:
      हाल OpCode_Weight:
      हाल OpCode_PostScript:
      हाल OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_मूल्यावली_t::name_op_to_सूचकांक (op)] = env.arg_ढेर.निकालो_uint ();
	env.clear_args ();
	अवरोध;
      हाल OpCode_isFixedPitch:
      हाल OpCode_ItalicAngle:
      हाल OpCode_UnderlinePosition:
      हाल OpCode_UnderlineThickness:
      हाल OpCode_PaintType:
      हाल OpCode_CharstringType:
      हाल OpCode_UniqueID:
      हाल OpCode_StrokeWidth:
      हाल OpCode_SyntheticBase:
      हाल OpCode_CIDFontVersion:
      हाल OpCode_CIDFontRevision:
      हाल OpCode_CIDFontType:
      हाल OpCode_UIDBase:
      हाल OpCode_FontBBox:
      हाल OpCode_XUID:
      हाल OpCode_BaseFontBlend:
	env.clear_args ();
	अवरोध;

      हाल OpCode_CIDCount:
	dictval.cid_गिनती = env.arg_ढेर.निकालो_uint ();
	env.clear_args ();
	अवरोध;

      हाल OpCode_ROS:
	dictval.ros_supplement = env.arg_ढेर.निकालो_uint ();
	dictval.nameSIDs[name_dict_मूल्यावली_t::ordering] = env.arg_ढेर.निकालो_uint ();
	dictval.nameSIDs[name_dict_मूल्यावली_t::registry] = env.arg_ढेर.निकालो_uint ();
	env.clear_args ();
	अगर (unlikely (env.in_error ())) वापस;
	निश्चित (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	अवरोध;

      हाल OpCode_Encoding:
	dictval.EncodingOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अगर (unlikely (dictval.EncodingOffset == 0)) वापस;
	अवरोध;

      हाल OpCode_charset:
	dictval.CharsetOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अगर (unlikely (dictval.CharsetOffset == 0)) वापस;
	अवरोध;

      हाल OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;

      हाल OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ढेर.निकालो_int ();
	dictval.privateDictInfo.आकार = env.arg_ढेर.निकालो_uint ();
	env.clear_args ();
	अवरोध;

      शेष:
	env.last_offset = env.str_ref.get_offset ();
	शीर्ष_dict_opset_t<cff1_शीर्ष_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	अगर (!env.arg_ढेर.is_empty ()) वापस;
	अवरोध;
    }

    अगर (unlikely (env.in_error ())) वापस;

    dictval.add_op (op, env.str_ref, val);
  }
};

काष्ठा cff1_font_dict_मूल्यावली_t : dict_मूल्यावली_t<op_str_t>
{
  व्योम init ()
  {
    dict_मूल्यावली_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  व्योम fini () { dict_मूल्यावली_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  अचिन्हित पूर्णांक    fontName;
};

काष्ठा cff1_font_dict_opset_t : dict_opset_t
{
  अटल व्योम process_op (op_code_t op, अंक_interp_env_t& env, cff1_font_dict_मूल्यावली_t& dictval)
  {
    चयन (op) {
      हाल OpCode_FontName:
	dictval.fontName = env.arg_ढेर.निकालो_uint ();
	env.clear_args ();
	अवरोध;
      हाल OpCode_FontMatrix:
      हाल OpCode_PaintType:
	env.clear_args ();
	अवरोध;
      हाल OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ढेर.निकालो_uint ();
	dictval.privateDictInfo.आकार = env.arg_ढेर.निकालो_uint ();
	env.clear_args ();
	अवरोध;

      शेष:
	dict_opset_t::process_op (op, env);
	अगर (!env.arg_ढेर.is_empty ()) वापस;
	अवरोध;
    }

    अगर (unlikely (env.in_error ())) वापस;

    dictval.add_op (op, env.str_ref);
  }
};

ढाँचा <typename VAL>
काष्ठा cff1_private_dict_मूल्यावली_base_t : dict_मूल्यावली_t<VAL>
{
  व्योम init ()
  {
    dict_मूल्यावली_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  व्योम fini () { dict_मूल्यावली_t<VAL>::fini (); }

  पूर्णांक                 subrsOffset;
  स्थिर CFF1Subrs    *localSubrs;
};

प्रकार cff1_private_dict_मूल्यावली_base_t<op_str_t> cff1_private_dict_मूल्यावली_subset_t;
प्रकार cff1_private_dict_मूल्यावली_base_t<अंक_dict_val_t> cff1_private_dict_मूल्यावली_t;

काष्ठा cff1_private_dict_opset_t : dict_opset_t
{
  अटल व्योम process_op (op_code_t op, अंक_interp_env_t& env, cff1_private_dict_मूल्यावली_t& dictval)
  {
    अंक_dict_val_t val;
    val.init ();

    चयन (op) {
      हाल Op_Code_Blue_मूल्यावली:
      हाल OpCode_OtherBlues:
      हाल OpCode_FamilyBlues:
      हाल OpCode_FamilyOtherBlues:
      हाल OpCode_StemSnapH:
      हाल OpCode_StemSnapV:
      हाल OpCode_StdHW:
      हाल OpCode_StdVW:
      हाल OpCode_BlueScale:
      हाल OpCode_BlueShift:
      हाल OpCode_BlueFuzz:
      हाल OpCode_ForceBold:
      हाल OpCode_LanguageGroup:
      हाल OpCode_ExpansionFactor:
      हाल OpCode_initialRandomSeed:
      हाल OpCode_defaultWidthX:
      हाल OpCode_nominalWidthX:
	env.clear_args ();
	अवरोध;
      हाल OpCode_Subrs:
	dictval.subrsOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;

      शेष:
	dict_opset_t::process_op (op, env);
	अगर (!env.arg_ढेर.is_empty ()) वापस;
	अवरोध;
    }

    अगर (unlikely (env.in_error ())) वापस;

    dictval.add_op (op, env.str_ref, val);
  }
};

काष्ठा cff1_private_dict_opset_subset_t : dict_opset_t
{
  अटल व्योम process_op (op_code_t op, अंक_interp_env_t& env, cff1_private_dict_मूल्यावली_subset_t& dictval)
  {
    चयन (op) {
      हाल Op_Code_Blue_मूल्यावली:
      हाल OpCode_OtherBlues:
      हाल OpCode_FamilyBlues:
      हाल OpCode_FamilyOtherBlues:
      हाल OpCode_StemSnapH:
      हाल OpCode_StemSnapV:
      हाल OpCode_StdHW:
      हाल OpCode_StdVW:
      हाल OpCode_BlueScale:
      हाल OpCode_BlueShift:
      हाल OpCode_BlueFuzz:
      हाल OpCode_ForceBold:
      हाल OpCode_LanguageGroup:
      हाल OpCode_ExpansionFactor:
      हाल OpCode_initialRandomSeed:
      हाल OpCode_defaultWidthX:
      हाल OpCode_nominalWidthX:
	env.clear_args ();
	अवरोध;

      हाल OpCode_Subrs:
	dictval.subrsOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;

      शेष:
	dict_opset_t::process_op (op, env);
	अगर (!env.arg_ढेर.is_empty ()) वापस;
	अवरोध;
    }

    अगर (unlikely (env.in_error ())) वापस;

    dictval.add_op (op, env.str_ref);
  }
};

प्रकार dict_interpreter_t<cff1_शीर्ष_dict_opset_t, cff1_शीर्ष_dict_मूल्यावली_t, cff1_शीर्ष_dict_interp_env_t> cff1_शीर्ष_dict_interpreter_t;
प्रकार dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_मूल्यावली_t> cff1_font_dict_interpreter_t;

प्रकार CFF1_सूचकांक CFF1_Name_सूचकांक;
प्रकार CFF1_सूचकांक CFF1_शीर्ष_Dict_सूचकांक;

काष्ठा cff1_font_dict_मूल्यावली_mod_t
{
  cff1_font_dict_मूल्यावली_mod_t() { init (); }

  व्योम init () { init ( &Null (cff1_font_dict_मूल्यावली_t), CFF_UNDEF_SID ); }

  व्योम init (स्थिर cff1_font_dict_मूल्यावली_t *base_,
	     अचिन्हित पूर्णांक fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  अचिन्हित get_गिनती () स्थिर { वापस base->get_गिनती (); }

  स्थिर op_str_t &चालक [] (अचिन्हित पूर्णांक i) स्थिर { वापस (*base)[i]; }

  स्थिर cff1_font_dict_मूल्यावली_t    *base;
  table_info_t		   privateDictInfo;
  अचिन्हित पूर्णांक		fontName;
};

काष्ठा CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  ढाँचा <typename ITER, typename OP_SERIALIZER>
  bool serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { वापस FDArray<HBUINT16>::serialize<cff1_font_dict_मूल्यावली_mod_t, cff1_font_dict_मूल्यावली_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

namespace OT {

using namespace CFF;

काष्ठा cff1
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  ढाँचा <typename PRIVOPSET, typename PRIVDICTVAL>
  काष्ठा accelerator_templ_t
  {
    अटल constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      अगर (!face) वापस;

      शीर्ष_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      यह->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (यह->blob);
      sc.start_processing ();

      स्थिर OT::cff1 *cff = यह->blob->ढाँचा as<OT::cff1> ();

      अगर (cff == &Null (OT::cff1))
        जाओ हार;

      name_सूचकांक = &cff->name_सूचकांक (cff);
      अगर ((name_सूचकांक == &Null (CFF1_Name_सूचकांक)) || !name_सूचकांक->sanitize (&sc))
        जाओ हार;
      hb_barrier ();

      शीर्ष_Dict_सूचकांक = &StructAtOffsetOrNull<CFF1_शीर्ष_Dict_सूचकांक> (name_सूचकांक, name_सूचकांक->get_आकार (), sc);
      अगर (शीर्ष_Dict_सूचकांक == &Null (CFF1_शीर्ष_Dict_सूचकांक) || (शीर्ष_Dict_सूचकांक->गिनती == 0))
        जाओ हार;
      hb_barrier ();

      { /* parse top dict */
	स्थिर hb_ubytes_t शीर्ष_Dict_Str = (*शीर्ष_Dict_सूचकांक)[0];
	अगर (unlikely (!शीर्ष_Dict_Str.sanitize (&sc)))   जाओ हार;
	hb_barrier ();
	cff1_शीर्ष_dict_interp_env_t env (शीर्ष_Dict_Str);
	cff1_शीर्ष_dict_interpreter_t शीर्ष_interp (env);
	अगर (unlikely (!शीर्ष_interp.interpret (शीर्ष_Dict_2)))   जाओ हार;
      }

      अगर (is_predef_charset ())
	charset = &Null (Charset);
      अन्यथा
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, शीर्ष_Dict_2.CharsetOffset, sc, &अंक_charset_entries);
	अगर (unlikely (charset == &Null (Charset)))   जाओ हार;
      }

      fd_गिनती = 1;
      अगर (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, शीर्ष_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, शीर्ष_Dict_2.FDSelectOffset, sc, fdArray->गिनती);
	अगर (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  जाओ हार;

	fd_गिनती = fdArray->गिनती;
      }
      अन्यथा
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      अगर (is_CID ())
      {
	अगर (unlikely (charset == &Null (Charset)))   जाओ हार;
      }
      अन्यथा
      {
	अगर (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, शीर्ष_Dict_2.EncodingOffset, sc);
	  अगर (unlikely (encoding == &Null (Encoding)))   जाओ हार;
	}
      }

      string_सूचकांक = &StructAtOffsetOrNull<CFF1_String_सूचकांक> (शीर्ष_Dict_सूचकांक, शीर्ष_Dict_सूचकांक->get_आकार (), sc);
      अगर (string_सूचकांक == &Null (CFF1_String_सूचकांक))
        जाओ हार;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (string_सूचकांक, string_सूचकांक->get_आकार (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, शीर्ष_Dict_2.charStringsOffset, sc);
      अगर (charStrings == &Null (CFF1CharStrings))
        जाओ हार;

      अंक_glyphs = charStrings->गिनती;
      अगर (अंक_glyphs != sc.get_अंक_glyphs ())
        जाओ हार;

      अगर (unlikely (!privateDicts.resize (fd_गिनती)))
        जाओ हार;
      क्रम (अचिन्हित पूर्णांक i = 0; i < fd_गिनती; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      अगर (is_CID ())
      {
	क्रम (अचिन्हित पूर्णांक i = 0; i < fd_गिनती; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  अगर (unlikely (!fontDictStr.sanitize (&sc)))   जाओ हार;
	  hb_barrier ();
	  cff1_font_dict_मूल्यावली_t *font;
	  cff1_शीर्ष_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  अगर (unlikely (fontDicts.in_error ()))   जाओ हार;

	  font->init ();
	  अगर (unlikely (!font_interp.interpret (*font)))   जाओ हार;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  स्थिर hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.आकार).as_ubytes (font->privateDictInfo.आकार);
	  अगर (unlikely (font->privateDictInfo.आकार &&
			privDictStr == (स्थिर अचिन्हित अक्षर *) &Null (UnsizedByteStr))) जाओ हार;
	  अंक_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  अगर (unlikely (!priv_interp.interpret (*priv)))   जाओ हार;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      अन्यथा  /* non-CID */
      {
	cff1_शीर्ष_dict_मूल्यावली_t *font = &शीर्ष_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	स्थिर hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.आकार).as_ubytes (font->privateDictInfo.आकार);
	अगर (font->privateDictInfo.आकार &&
	    unlikely (privDictStr == (स्थिर अचिन्हित अक्षर *) &Null (UnsizedByteStr))) जाओ हार;
	अंक_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	अगर (unlikely (!priv_interp.interpret (*priv)))   जाओ हार;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      वापस;

      हार:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    व्योम _fini ()
    {
      sc.end_processing ();
      शीर्ष_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () स्थिर { वापस blob; }

    bool is_valid () स्थिर { वापस blob; }
    bool   is_CID () स्थिर { वापस शीर्ष_Dict_2.is_CID (); }

    bool is_predef_charset () स्थिर { वापस शीर्ष_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    अचिन्हित पूर्णांक std_code_to_glyph (hb_codepoint_t code) स्थिर
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      अगर (unlikely (sid == CFF_UNDEF_SID))
	वापस 0;

      अगर (charset != &Null (Charset))
	वापस charset->get_glyph (sid, अंक_glyphs);
      अन्यथा अगर ((शीर्ष_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) वापस sid;
      वापस 0;
    }

    bool is_predef_encoding () स्थिर { वापस शीर्ष_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) स्थिर
    {
      अगर (encoding != &Null (Encoding))
	वापस encoding->get_code (glyph);
      अन्यथा
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	अगर (sid == 0) वापस 0;
	hb_codepoint_t code = 0;
	चयन (शीर्ष_Dict_2.EncodingOffset)
	{
	हाल StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  अवरोध;
	हाल ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  अवरोध;
	शेष:
	  अवरोध;
	}
	वापस code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () स्थिर
    {
      अगर (charset != &Null (Charset))
      {
	स्वतः *mapping = (glyph_to_sid_map_t *) hb_malloc (माप (glyph_to_sid_map_t));
	अगर (unlikely (!mapping)) वापस nullptr;
	mapping = नया (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, अंक_glyphs);
	वापस mapping;
      }
      अन्यथा
	वापस nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) स्थिर
    {
      अगर (charset != &Null (Charset))
	वापस charset->get_sid (glyph, अंक_glyphs, cache);
      अन्यथा
      {
	hb_codepoint_t sid = 0;
	चयन (शीर्ष_Dict_2.CharsetOffset)
	{
	  हाल ISOAdobeCharset:
	    अगर (glyph <= 228 /*zcaron*/) sid = glyph;
	    अवरोध;
	  हाल ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    अवरोध;
	  हाल ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    अवरोध;
	  शेष:
	    अवरोध;
	}
	वापस sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) स्थिर
    {
      अगर (charset != &Null (Charset))
	वापस charset->get_glyph (sid, अंक_glyphs);
      अन्यथा
      {
	hb_codepoint_t glyph = 0;
	चयन (शीर्ष_Dict_2.CharsetOffset)
	{
	  हाल ISOAdobeCharset:
	    अगर (sid <= 228 /*zcaron*/) glyph = sid;
	    अवरोध;
	  हाल ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    अवरोध;
	  हाल ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    अवरोध;
	  शेष:
	    अवरोध;
	}
	वापस glyph;
      }
    }

    रक्षित:
    hb_sanitize_context_t   sc;

    खुला:
    hb_blob_t               *blob = nullptr;
    स्थिर Encoding	    *encoding = nullptr;
    स्थिर Charset	    *charset = nullptr;
    स्थिर CFF1_Name_सूचकांक     *name_सूचकांक = nullptr;
    स्थिर CFF1_शीर्ष_Dict_सूचकांक  *शीर्ष_Dict_सूचकांक = nullptr;
    स्थिर CFF1_String_सूचकांक   *string_सूचकांक = nullptr;
    स्थिर CFF1Subrs	    *globalSubrs = nullptr;
    स्थिर CFF1CharStrings   *charStrings = nullptr;
    स्थिर CFF1FDArray       *fdArray = nullptr;
    स्थिर CFF1FDSelect      *fdSelect = nullptr;
    अचिन्हित पूर्णांक	     fd_गिनती = 0;

    cff1_शीर्ष_dict_मूल्यावली_t   शीर्ष_Dict_2;
    hb_vector_t<cff1_font_dict_मूल्यावली_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    अचिन्हित पूर्णांक	     अंक_glyphs = 0;
    अचिन्हित पूर्णांक	     अंक_charset_entries = 0;
  };

  काष्ठा accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_मूल्यावली_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      अगर (!is_valid ()) वापस;
      अगर (is_CID ()) वापस;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      अगर (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    bool get_glyph_name (hb_codepoint_t glyph,
			 अक्षर *buf, अचिन्हित पूर्णांक buf_len) स्थिर
    {
      अगर (unlikely (glyph >= अंक_glyphs)) वापस false;
      अगर (unlikely (!is_valid ())) वापस false;
      अगर (is_CID()) वापस false;
      अगर (unlikely (!buf_len)) वापस true;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      स्थिर अक्षर *str;
      माप_प्रकार str_len;
      अगर (sid < cff1_std_strings_लंबाई)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.लंबाई;
      }
      अन्यथा
      {
	hb_ubytes_t ubyte_str = (*string_सूचकांक)[sid - cff1_std_strings_लंबाई];
	str = (स्थिर अक्षर *)ubyte_str.arrayZ;
	str_len = ubyte_str.लंबाई;
      }
      अगर (!str_len) वापस false;
      अचिन्हित पूर्णांक len = hb_min (buf_len - 1, str_len);
      म_नकलन (buf, (स्थिर अक्षर*)str, len);
      buf[len] = '\0';
      वापस true;
    }

    bool get_glyph_from_name (स्थिर अक्षर *name, पूर्णांक len,
			      hb_codepoint_t *glyph) स्थिर
    {
      अगर (unlikely (!is_valid ())) वापस false;
      अगर (is_CID()) वापस false;
      अगर (len < 0) len = म_माप (name);
      अगर (unlikely (!len)) वापस false;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      अगर (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, माप (hb_sorted_vector_t<gname_t>));
	अगर (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  क्रम (hb_codepoint_t gid = 0; gid < अंक_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    अगर (sid < cff1_std_strings_लंबाई)
	      gname.name = cff1_std_strings (sid);
	    अन्यथा
	    {
	      hb_ubytes_t	ustr = (*string_सूचकांक)[sid - cff1_std_strings_लंबाई];
	      gname.name = hb_bytes_t ((स्थिर अक्षर*) ustr.arrayZ, ustr.लंबाई);
	    }
	    अगर (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->क्विक ();
	}
	अगर (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  अगर (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  जाओ retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      स्थिर gname_t *gname = names ? names->द्वा_खोज (key) : nullptr;
      अगर (!gname) वापस false;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      अगर (!gid && gname->sid) वापस false;
      *glyph = gid;
      वापस true;
    }

    HB_INTERNAL bool get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) स्थिर;
    HB_INTERNAL bool get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) स्थिर;

    निजी:
    काष्ठा gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      अटल पूर्णांक cmp (स्थिर व्योम *a_, स्थिर व्योम *b_)
      {
	स्थिर gname_t *a = (स्थिर gname_t *)a_;
	स्थिर gname_t *b = (स्थिर gname_t *)b_;
	अचिन्हित minlen = hb_min (a->name.लंबाई, b->name.लंबाई);
	पूर्णांक ret = म_भेदन (a->name.arrayZ, b->name.arrayZ, minlen);
	अगर (ret) वापस ret;
	वापस a->name.लंबाई - b->name.लंबाई;
      }

      पूर्णांक cmp (स्थिर gname_t &a) स्थिर { वापस cmp (&a, यह); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    प्रकार accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_मूल्यावली_t> SUPER;
  };

  काष्ठा accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_मूल्यावली_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      अगर (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL bool subset (hb_subset_context_t *c) स्थिर;
    HB_INTERNAL bool serialize (hb_serialize_context_t *c,
				काष्ठा cff1_subset_plan &plan) स्थिर;
    HB_INTERNAL bool get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) स्थिर;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    प्रकार accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_मूल्यावली_subset_t> SUPER;
  };

  रक्षित:
  HB_INTERNAL अटल hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL अटल hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL अटल hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL अटल hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL अटल hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL अटल hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL अटल hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  खुला:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1_Name_सूचकांक, HBUINT8> name_सूचकांक; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_आकार;	  /* offset size (unused?) */

  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

काष्ठा cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  व्योम depend (hb_depend_तथ्य_builder_t *builder) स्थिर
  {
    अगर (!is_valid ()) वापस;
    क्रम (hb_codepoint_t gid = 0; gid < अंक_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      अगर (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#पूर्ण_अगर /* HB_OT_CFF1_TABLE_HH */
