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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_CFF1_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_CFF1_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-depend-data.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_NAME cff1_std_ਲੜੀਆਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-string-array.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STRING_ARRAY_LIST
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STRING_ARRAY_NAME

ਨਾਮਾਕਾਸ਼ CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_CFF1 HB_ਟੈਗ('C','F','F',' ')

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CFF_UNDEF_SID   CFF_UNDEF_ਕੋਡ

ਗਿਣਤੀ EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
ਗਿਣਤੀ CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

ਕਿਸਮਪਰਿਭਾਸ਼ਾ CFF1_ਸੂਚਕਾਂਕ          CFF1_Char_ਲੜੀਆਂ;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Subrs<HBUINT16>    CFF1Subrs;

ਬਣਤਰ CFF1FDSelect : FDSelect {};

/* Encoding */
ਬਣਤਰ Encoding0 {
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_ਕੋਡ (hb_codepoint_t glyph) ਅਚਲ
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (glyph > 0);
    glyph--;
    ਜੇ (glyph < nCodes ())
    {
      ਵਾਪਸ (hb_codepoint_t)codes[glyph];
    }
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ CFF_UNDEF_ਕੋਡ;
  }

  HBUINT8 &nCodes () { ਵਾਪਸ codes.len; }
  HBUINT8 nCodes () ਅਚਲ { ਵਾਪਸ codes.len; }

  Array_ਦਾ<HBUINT8, HBUINT8> codes;

  DEFINE_ਮਾਪ_ARRAY_SIZED (1, codes);
};

ਬਣਤਰ Encoding1_Range {
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT8   ਪਹਿਲਾ;
  HBUINT8   n_ਖੱਬਾ;

  DEFINE_ਮਾਪ_STATIC (2);
};

ਬਣਤਰ Encoding1 {
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_ਕੋਡ (hb_codepoint_t glyph) ਅਚਲ
  {
    /* TODO: Add cache like get_sid. */
    ਪ੍ਰਤਿਜ੍ਞਾ (glyph > 0);
    glyph--;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < nRanges (); i++)
    {
      ਜੇ (glyph <= ranges[i].n_ਖੱਬਾ)
      {
	hb_codepoint_t ਕੋਡ = (hb_codepoint_t) ranges[i].ਪਹਿਲਾ + glyph;
	ਵਾਪਸ (likely (ਕੋਡ < 0x100) ? ਕੋਡ: CFF_UNDEF_ਕੋਡ);
      }
      glyph -= (ranges[i].n_ਖੱਬਾ + 1);
    }
    ਵਾਪਸ CFF_UNDEF_ਕੋਡ;
  }

  HBUINT8 &nRanges () { ਵਾਪਸ ranges.len; }
  HBUINT8 nRanges () ਅਚਲ { ਵਾਪਸ ranges.len; }

  Array_ਦਾ<Encoding1_Range, HBUINT8> ranges;

  DEFINE_ਮਾਪ_ARRAY_SIZED (1, ranges);
};

ਬਣਤਰ SuppEncoding {
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT8   ਕੋਡ;
  HBUINT16  glyph;

  DEFINE_ਮਾਪ_STATIC (3);
};

ਬਣਤਰ CFF1_Supp_Enc_ਅੰਕੜੇ {
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (supps.sanitize (c));
  }

  ਖਾਲੀ get_codes (hb_codepoint_t sid, hb_ਸਦਿਸ਼_t<hb_codepoint_t> &codes) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < nSups (); i++)
      ਜੇ (sid == supps[i].glyph)
	codes.push (supps[i].ਕੋਡ);
  }

  HBUINT8 &nSups () { ਵਾਪਸ supps.len; }
  HBUINT8 nSups () ਅਚਲ { ਵਾਪਸ supps.len; }

  Array_ਦਾ<SuppEncoding, HBUINT8> supps;

  DEFINE_ਮਾਪ_ARRAY_SIZED (1, supps);
};

ਬਣਤਰ Encoding
{
  /* serialize a fullset Encoding */
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ Encoding &src)
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ enc_ਗਿਣਤੀ,
		  ਅਚਲ hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t>& ਕੋਡ_ranges,
		  ਅਚਲ hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (ਇਹ);
    Encoding *dest = c->extend_min (ਇਹ);
    ਜੇ (unlikely (!dest)) return_trace (ਝੂਠ);
    dest->format = format | ((supp_codes.ਲੰਮਾਈ > 0) ? 0x80 : 0);
    ਚੋਣ (format) {
    ਹਾਲਤ 0:
    {
      Encoding0 *fmt0 = c->allocate_ਮਾਪ<Encoding0> (Encoding0::min_ਮਾਪ + HBUINT8::static_ਮਾਪ * enc_ਗਿਣਤੀ);
      ਜੇ (unlikely (!fmt0)) return_trace (ਝੂਠ);
      fmt0->nCodes () = enc_ਗਿਣਤੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਕੋਡ_ranges.ਲੰਮਾਈ; i++)
      {
	hb_codepoint_t ਕੋਡ = ਕੋਡ_ranges[i].ਕੋਡ;
	ਲਈ (ਪੂਰਨ ਖੱਬੇ = (ਪੂਰਨ)ਕੋਡ_ranges[i].glyph; ਖੱਬੇ >= 0; ਖੱਬੇ--)
	  fmt0->codes[glyph++] = ਕੋਡ++;
	ਜੇ (unlikely (!((glyph <= 0x100) && (ਕੋਡ <= 0x100))))
	  return_trace (ਝੂਠ);
      }
    }
    ਰੋਕੋ;

    ਹਾਲਤ 1:
    {
      Encoding1 *fmt1 = c->allocate_ਮਾਪ<Encoding1> (Encoding1::min_ਮਾਪ + Encoding1_Range::static_ਮਾਪ * ਕੋਡ_ranges.ਲੰਮਾਈ);
      ਜੇ (unlikely (!fmt1)) return_trace (ਝੂਠ);
      fmt1->nRanges () = ਕੋਡ_ranges.ਲੰਮਾਈ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਕੋਡ_ranges.ਲੰਮਾਈ; i++)
      {
	ਜੇ (unlikely (!((ਕੋਡ_ranges[i].ਕੋਡ <= 0xFF) && (ਕੋਡ_ranges[i].glyph <= 0xFF))))
	  return_trace (ਝੂਠ);
	fmt1->ranges[i].ਪਹਿਲਾ = ਕੋਡ_ranges[i].ਕੋਡ;
	fmt1->ranges[i].n_ਖੱਬਾ = ਕੋਡ_ranges[i].glyph;
      }
    }
    ਰੋਕੋ;

    }

    ਜੇ (supp_codes.ਲੰਮਾਈ)
    {
      CFF1_Supp_Enc_ਅੰਕੜੇ *supp_ਅੰਕੜੇ = c->allocate_ਮਾਪ<CFF1_Supp_Enc_ਅੰਕੜੇ> (CFF1_Supp_Enc_ਅੰਕੜੇ::min_ਮਾਪ + SuppEncoding::static_ਮਾਪ * supp_codes.ਲੰਮਾਈ);
      ਜੇ (unlikely (!supp_ਅੰਕੜੇ)) return_trace (ਝੂਠ);
      supp_ਅੰਕੜੇ->nSups () = supp_codes.ਲੰਮਾਈ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < supp_codes.ਲੰਮਾਈ; i++)
      {
	supp_ਅੰਕੜੇ->supps[i].ਕੋਡ = supp_codes[i].ਕੋਡ;
	supp_ਅੰਕੜੇ->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (ਸੱਚ);
  }

  size_t get_ਮਾਪ () ਅਚਲ
  {
    size_t ਮਾਪ = min_ਮਾਪ;
    ਚੋਣ (ਸਾਰਣੀ_format ())
    {
    ਹਾਲਤ 0: hb_barrier (); ਮਾਪ = hb_unsigned_ਜੋੜੋ_saturate (ਮਾਪ, u.format0.get_ਮਾਪ ()); ਰੋਕੋ;
    ਹਾਲਤ 1: hb_barrier (); ਮਾਪ = hb_unsigned_ਜੋੜੋ_saturate (ਮਾਪ, u.format1.get_ਮਾਪ ()); ਰੋਕੋ;
    }
    ਜੇ (has_supplement ())
      ਮਾਪ = hb_unsigned_ਜੋੜੋ_saturate (ਮਾਪ, supp_Enc_ਅੰਕੜੇ ().get_ਮਾਪ ());
    ਵਾਪਸ ਮਾਪ;
  }

  hb_codepoint_t get_ਕੋਡ (hb_codepoint_t glyph) ਅਚਲ
  {
    ਚੋਣ (ਸਾਰਣੀ_format ())
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_ਕੋਡ (glyph);
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_ਕੋਡ (glyph);
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  uint8_t ਸਾਰਣੀ_format () ਅਚਲ { ਵਾਪਸ format & 0x7F; }
  ਤਾਰਕਿਕ  has_supplement () ਅਚਲ { ਵਾਪਸ format & 0x80; }

  ਖਾਲੀ get_supplement_codes (hb_codepoint_t sid, hb_ਸਦਿਸ਼_t<hb_codepoint_t> &codes) ਅਚਲ
  {
    codes.clear ();
    ਜੇ (has_supplement ())
      supp_Enc_ਅੰਕੜੇ().get_codes (sid, codes);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਚੋਣ (ਸਾਰਣੀ_format ())
    {
    ਹਾਲਤ 0: hb_barrier (); ਜੇ (unlikely (!u.format0.sanitize (c))) { return_trace (ਝੂਠ); } ਰੋਕੋ;
    ਹਾਲਤ 1: hb_barrier (); ਜੇ (unlikely (!u.format1.sanitize (c))) { return_trace (ਝੂਠ); } ਰੋਕੋ;
    ਮੂਲ:return_trace (ਝੂਠ);
    }
    return_trace (likely (!has_supplement () || supp_Enc_ਅੰਕੜੇ ().sanitize (c)));
  }

  ਸੁਰੱਖਿਅਤ:
  ਅਚਲ CFF1_Supp_Enc_ਅੰਕੜੇ &supp_Enc_ਅੰਕੜੇ () ਅਚਲ
  {
    ਚੋਣ (ਸਾਰਣੀ_format ())
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ StructAfter<CFF1_Supp_Enc_ਅੰਕੜੇ> (u.format0.codes[u.format0.nCodes ()-1]);
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ StructAfter<CFF1_Supp_Enc_ਅੰਕੜੇ> (u.format1.ranges[u.format1.nRanges ()-1]);
    ਮੂਲ:ਵਾਪਸ Null (CFF1_Supp_Enc_ਅੰਕੜੇ);
    }
  }

  ਜਨਤਕ:
  HBUINT8	format;
  ਸੰਘ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_ਮਾਪ_MIN (1);
};

/* Charset */
ਬਣਤਰ Charset0
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਅੰਕ_charset_entries) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (ਅੰਕ_charset_entries) *ਅੰਕ_charset_entries = ਅੰਕ_glyphs;
    return_trace (sids.sanitize (c, ਅੰਕ_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਜੇ (unlikely (glyph >= ਅੰਕ_glyphs)) ਵਾਪਸ 0;
    ਜੇ (unlikely (glyph == 0))
      ਵਾਪਸ 0;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ sids[glyph - 1];
  }

  ਖਾਲੀ collect_glyph_ਮੰਜ਼ਿਲ_sid_map (glyph_ਮੰਜ਼ਿਲ_sid_map_t *mapping, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਜੇ (!mapping->resize_dirty  (ਅੰਕ_glyphs)) ਵਾਪਸ;
    ਲਈ (hb_codepoint_t gid = 1; gid < ਅੰਕ_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਜੇ (sid == 0)
      ਵਾਪਸ 0;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph = 1; glyph < ਅੰਕ_glyphs; glyph++)
    {
      ਜੇ (sids[glyph-1] == sid)
	ਵਾਪਸ glyph;
    }
    ਵਾਪਸ 0;
  }

  ਸਥਿਰ size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (ਅੰਕ_glyphs > 0);
    ਵਾਪਸ Unsized_Array_ਦਾ<HBUINT16>::get_ਮਾਪ (ਅੰਕ_glyphs - 1);
  }

  Unsized_Array_ਦਾ<HBUINT16> sids;

  DEFINE_ਮਾਪ_ARRAY(0, sids);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TYPE>
ਬਣਤਰ Charset_Range {
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT16  ਪਹਿਲਾ;
  TYPE      n_ਖੱਬਾ;

  DEFINE_ਮਾਪ_STATIC (HBUINT16::static_ਮਾਪ + TYPE::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TYPE>
ਬਣਤਰ Charset1_2 {
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਅੰਕ_charset_entries) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਅੰਕ_glyphs--;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਲਈ (i = 0; ਅੰਕ_glyphs > 0; i++)
    {
      ਜੇ (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (ਅੰਕ_glyphs >= ranges[i].n_ਖੱਬਾ + 1))))
	return_trace (ਝੂਠ);
      ਅੰਕ_glyphs -= (ranges[i].n_ਖੱਬਾ + 1);
    }
    ਜੇ (ਅੰਕ_charset_entries)
      *ਅੰਕ_charset_entries = i;
    return_trace (ਸੱਚ);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs,
			  ਕੋਡ_pair_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (unlikely (glyph >= ਅੰਕ_glyphs)) ਵਾਪਸ 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    hb_codepoint_t ਸ਼ੁਰੂ_glyph;
    ਜੇ (cache && likely (cache->glyph <= glyph))
    {
      i = cache->ਕੋਡ;
      ਸ਼ੁਰੂ_glyph = cache->glyph;
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (unlikely (glyph == 0)) ਵਾਪਸ 0;
      i = 0;
      ਸ਼ੁਰੂ_glyph = 1;
    }
    glyph -= ਸ਼ੁਰੂ_glyph;
    ਲਈ (;; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ranges[i].n_ਖੱਬਾ;
      ਜੇ (glyph <= ਗਿਣਤੀ_2)
      {
        ਜੇ (cache)
	  *cache = {i, ਸ਼ੁਰੂ_glyph};
	ਵਾਪਸ ranges[i].ਪਹਿਲਾ + glyph;
      }
      ਗਿਣਤੀ_2++;
      ਸ਼ੁਰੂ_glyph += ਗਿਣਤੀ_2;
      glyph -= ਗਿਣਤੀ_2;
    }

    ਵਾਪਸ 0;
  }

  ਖਾਲੀ collect_glyph_ਮੰਜ਼ਿਲ_sid_map (glyph_ਮੰਜ਼ਿਲ_sid_map_t *mapping, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਜੇ (!mapping->resize_dirty  (ਅੰਕ_glyphs)) ਵਾਪਸ;

    hb_codepoint_t gid = 1;
    ਜੇ (gid >= ਅੰਕ_glyphs)
      ਵਾਪਸ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].ਪਹਿਲਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ranges[i].n_ਖੱਬਾ + 1;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ = gid + ਗਿਣਤੀ_2;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਗਿਣਤੀ_2; j++)
	mapping->arrayZ[gid++] = {sid++, ਆਖਰੀ - 1};

      ਜੇ (gid >= ਅੰਕ_glyphs)
        ਰੋਕੋ;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਜੇ (sid == 0) ਵਾਪਸ 0;
    hb_codepoint_t  glyph = 1;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;; i++)
    {
      ਜੇ (glyph >= ਅੰਕ_glyphs)
	ਵਾਪਸ 0;
      ਜੇ ((ranges[i].ਪਹਿਲਾ <= sid) && (sid <= ranges[i].ਪਹਿਲਾ + ranges[i].n_ਖੱਬਾ))
	ਵਾਪਸ glyph + (sid - ranges[i].ਪਹਿਲਾ);
      glyph += (ranges[i].n_ਖੱਬਾ + 1);
    }

    ਵਾਪਸ 0;
  }

  size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਪੂਰਨ glyph = (ਪੂਰਨ) ਅੰਕ_glyphs;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ranges = 0;

    ਪ੍ਰਤਿਜ੍ਞਾ (glyph > 0);
    glyph--;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_ਖੱਬਾ + 1);
      ਅੰਕ_ranges++;
    }

    ਵਾਪਸ get_ਮਾਪ_for_ranges (ਅੰਕ_ranges);
  }

  ਸਥਿਰ size_t get_ਮਾਪ_for_ranges (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ranges)
  {
    ਵਾਪਸ Unsized_Array_ਦਾ<Charset_Range<TYPE> >::get_ਮਾਪ (ਅੰਕ_ranges);
  }

  Unsized_Array_ਦਾ<Charset_Range<TYPE>> ranges;

  DEFINE_ਮਾਪ_ARRAY (0, ranges);
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ Charset1_2<HBUINT8>     Charset1;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Charset1_2<HBUINT16>    Charset2;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Charset_Range<HBUINT8>  Charset1_Range;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Charset_Range<HBUINT16> Charset2_Range;

ਬਣਤਰ Charset
{
  /* serialize a fullset Charset */
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ Charset &src, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed ((ਅਚਲ ਅੱਖਰ *) &src, src.get_ਮਾਪ (ਅੰਕ_glyphs)));
  }

  /* serialize a subset Charset */
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
		  ਅਚਲ hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (ਇਹ);
    Charset *dest = c->extend_min (ਇਹ);
    ਜੇ (unlikely (!dest)) return_trace (ਝੂਠ);
    dest->format = format;
    ਚੋਣ (format)
    {
    ਹਾਲਤ 0:
    {
      Charset0 *fmt0 = c->allocate_ਮਾਪ<Charset0> (Charset0::get_ਮਾਪ (ਅੰਕ_glyphs), ਝੂਠ);
      ਜੇ (unlikely (!fmt0)) return_trace (ਝੂਠ);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < sid_ranges.ਲੰਮਾਈ; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].ਕੋਡ;
	ਲਈ (ਪੂਰਨ ਖੱਬੇ = (ਪੂਰਨ)sid_ranges.arrayZ[i].glyph; ਖੱਬੇ >= 0; ਖੱਬੇ--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    ਰੋਕੋ;

    ਹਾਲਤ 1:
    {
      Charset1 *fmt1 = c->allocate_ਮਾਪ<Charset1> (Charset1::get_ਮਾਪ_for_ranges (sid_ranges.ਲੰਮਾਈ), ਝੂਠ);
      ਜੇ (unlikely (!fmt1)) return_trace (ਝੂਠ);
      hb_codepoint_t all_glyphs = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < sid_ranges.ਲੰਮਾਈ; i++)
      {
        ਸਵੈਚਾਲਿਤ &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].ਪਹਿਲਾ = _.ਕੋਡ;
	fmt1->ranges[i].n_ਖੱਬਾ = _.glyph;
      }
      ਜੇ (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (ਝੂਠ);
    }
    ਰੋਕੋ;

    ਹਾਲਤ 2:
    {
      Charset2 *fmt2 = c->allocate_ਮਾਪ<Charset2> (Charset2::get_ਮਾਪ_for_ranges (sid_ranges.ਲੰਮਾਈ), ਝੂਠ);
      ਜੇ (unlikely (!fmt2)) return_trace (ਝੂਠ);
      hb_codepoint_t all_glyphs = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < sid_ranges.ਲੰਮਾਈ; i++)
      {
        ਸਵੈਚਾਲਿਤ &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].ਪਹਿਲਾ = _.ਕੋਡ;
	fmt2->ranges[i].n_ਖੱਬਾ = _.glyph;
      }
      ਜੇ (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (ਝੂਠ);
    }
    ਰੋਕੋ;

    }
    return_trace (ਸੱਚ);
  }

  size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (min_ਮਾਪ, u.format0.get_ਮਾਪ (ਅੰਕ_glyphs));
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (min_ਮਾਪ, u.format1.get_ਮਾਪ (ਅੰਕ_glyphs));
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (min_ਮਾਪ, u.format2.get_ਮਾਪ (ਅੰਕ_glyphs));
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
			  ਕੋਡ_pair_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_sid (glyph, ਅੰਕ_glyphs);
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_sid (glyph, ਅੰਕ_glyphs, cache);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_sid (glyph, ਅੰਕ_glyphs, cache);
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਖਾਲੀ collect_glyph_ਮੰਜ਼ਿਲ_sid_map (glyph_ਮੰਜ਼ਿਲ_sid_map_t *mapping, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); u.format0.collect_glyph_ਮੰਜ਼ਿਲ_sid_map (mapping, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 1: hb_barrier (); u.format1.collect_glyph_ਮੰਜ਼ਿਲ_sid_map (mapping, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 2: hb_barrier (); u.format2.collect_glyph_ਮੰਜ਼ਿਲ_sid_map (mapping, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਮੂਲ:ਵਾਪਸ;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_glyph (sid, ਅੰਕ_glyphs);
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_glyph (sid, ਅੰਕ_glyphs);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_glyph (sid, ਅੰਕ_glyphs);
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਅੰਕ_charset_entries) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_ਅੰਕ_glyphs (), ਅੰਕ_charset_entries));
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_ਅੰਕ_glyphs (), ਅੰਕ_charset_entries));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_ਅੰਕ_glyphs (), ਅੰਕ_charset_entries));
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  HBUINT8       format;
  ਸੰਘ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_ਮਾਪ_MIN (1);
};

ਬਣਤਰ CFF1_String_ਸੂਚਕਾਂਕ : CFF1_ਸੂਚਕਾਂਕ
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ CFF1_String_ਸੂਚਕਾਂਕ &ਲੜੀਆਂ,
		  ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &sidmap)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely ((ਲੜੀਆਂ.ਗਿਣਤੀ_2 == 0) || (sidmap.ਲੰਮਾਈ == 0)))
    {
      ਜੇ (unlikely (!c->extend_min (ਇਹ->ਗਿਣਤੀ_2)))
	return_trace (ਝੂਠ);
      ਗਿਣਤੀ_2 = 0;
      return_trace (ਸੱਚ);
    }

    ਜੇ (unlikely (sidmap.ਵਿੱਚ_error ())) return_trace (ਝੂਠ);

    // Save this in a vector since serialize() iterates it twice.
    hb_ਸਦਿਸ਼_t<hb_ubytes_t> ਬਾਈਟਾਂ_Array (+ hb_iter (sidmap)
					 | hb_map (ਲੜੀਆਂ));

    ਜੇ (unlikely (ਬਾਈਟਾਂ_Array.ਵਿੱਚ_error ())) return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ਨਤੀਜਾ = CFF1_ਸੂਚਕਾਂਕ::serialize (c, ਬਾਈਟਾਂ_Array);
    return_trace (ਨਤੀਜਾ);
  }
};

ਬਣਤਰ cff1_ਸਿਖਰ_dict_interp_env_t : ਅੰਕ_interp_env_t
{
  cff1_ਸਿਖਰ_dict_interp_env_t ()
    : ਅੰਕ_interp_env_t(), prev_ਵਿੱਥ(0), ਆਖਰੀ_ਵਿੱਥ(0) {}
  cff1_ਸਿਖਰ_dict_interp_env_t (ਅਚਲ hb_ubytes_t &ਬਾਈਟਾਂ)
    : ਅੰਕ_interp_env_t(ਬਾਈਟਾਂ), prev_ਵਿੱਥ(0), ਆਖਰੀ_ਵਿੱਥ(0) {}

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prev_ਵਿੱਥ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_ਵਿੱਥ;
};

ਬਣਤਰ name_dict_ਮੁੱਲਾਂ_t
{
  ਗਿਣਤੀ name_dict_val_ਸੂਚਕਾਂਕ_t
  {
      ਸੰਸਕਰਣ,
      notice,
      copyright,
      ਭਰਿਆ_Name,
      familyName,
      ਭਾਰ,
      postscript,
      fontName,
      ਅਧਾਰ_Font_Name,
      registry,
      ordering,

      Val_ਗਿਣਤੀ
  };

  ਖਾਲੀ init ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < Val_ਗਿਣਤੀ; i++)
      values[i] = CFF_UNDEF_SID;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ& ਸੰਚਾਲਕ[] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  { ਪ੍ਰਤਿਜ੍ਞਾ (i < Val_ਗਿਣਤੀ); ਵਾਪਸ values[i]; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੰਚਾਲਕ[] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਪ੍ਰਤਿਜ੍ਞਾ (i < Val_ਗਿਣਤੀ); ਵਾਪਸ values[i]; }

  ਸਥਿਰ ਗਿਣਤੀ name_dict_val_ਸੂਚਕਾਂਕ_t name_op_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (op_ਕੋਡ_t op)
  {
    ਚੋਣ (op) {
      ਮੂਲ: // can't happen - just make some compiler happy
      ਹਾਲਤ Op_ਕੋਡ_ਸੰਸਕਰਣ:
	ਵਾਪਸ ਸੰਸਕਰਣ;
      ਹਾਲਤ Op_ਕੋਡ_Notice:
	ਵਾਪਸ notice;
      ਹਾਲਤ Op_ਕੋਡ_Copyright:
	ਵਾਪਸ copyright;
      ਹਾਲਤ Op_ਕੋਡ_ਭਰਿਆ_Name:
	ਵਾਪਸ ਭਰਿਆ_Name;
      ਹਾਲਤ Op_ਕੋਡ_Family_Name:
	ਵਾਪਸ familyName;
      ਹਾਲਤ Op_ਕੋਡ_ਭਾਰ:
	ਵਾਪਸ ਭਾਰ;
      ਹਾਲਤ Op_ਕੋਡ_Post_Script:
	ਵਾਪਸ postscript;
      ਹਾਲਤ Op_ਕੋਡ_Font_Name:
	ਵਾਪਸ fontName;
      ਹਾਲਤ Op_ਕੋਡ_ਅਧਾਰ_Font_Name:
	ਵਾਪਸ ਅਧਾਰ_Font_Name;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  values[Val_ਗਿਣਤੀ];
};

ਬਣਤਰ cff1_ਸਿਖਰ_dict_val_t : op_str_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਆਖਰੀ_arg_ਵਿੱਥ;
};

ਬਣਤਰ cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t : ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t<cff1_ਸਿਖਰ_dict_val_t>
{
  ਖਾਲੀ init ()
  {
    ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t<cff1_ਸਿਖਰ_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_ਗਿਣਤੀ = 8720;
    Encoding_ਵਿੱਥ = 0;
    Charset_ਵਿੱਥ = 0;
    FDSelect_ਵਿੱਥ = 0;
    ਨਿੱਜੀ_Dict_Info.init ();
  }
  ਖਾਲੀ fini () { ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t<cff1_ਸਿਖਰ_dict_val_t>::fini (); }

  ਤਾਰਕਿਕ is_CID () ਅਚਲ
  { ਵਾਪਸ nameSIDs[name_dict_ਮੁੱਲਾਂ_t::registry] != CFF_UNDEF_SID; }

  name_dict_ਮੁੱਲਾਂ_t  nameSIDs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ros_supplement_ਵਿੱਥ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ros_supplement;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    cid_ਗਿਣਤੀ;

  ਪੂਰਨ             Encoding_ਵਿੱਥ;
  ਪੂਰਨ             Charset_ਵਿੱਥ;
  ਪੂਰਨ             FDSelect_ਵਿੱਥ;
  ਸਾਰਣੀ_info_t    ਨਿੱਜੀ_Dict_Info;
};

ਬਣਤਰ cff1_ਸਿਖਰ_dict_opset_t : ਸਿਖਰ_dict_opset_t<cff1_ਸਿਖਰ_dict_val_t>
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff1_ਸਿਖਰ_dict_interp_env_t& env, cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t& dictval)
  {
    cff1_ਸਿਖਰ_dict_val_t  val {};

    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_ਸੰਸਕਰਣ:
      ਹਾਲਤ Op_ਕੋਡ_Notice:
      ਹਾਲਤ Op_ਕੋਡ_Copyright:
      ਹਾਲਤ Op_ਕੋਡ_ਭਰਿਆ_Name:
      ਹਾਲਤ Op_ਕੋਡ_Font_Name:
      ਹਾਲਤ Op_ਕੋਡ_Family_Name:
      ਹਾਲਤ Op_ਕੋਡ_ਭਾਰ:
      ਹਾਲਤ Op_ਕੋਡ_Post_Script:
      ਹਾਲਤ Op_ਕੋਡ_ਅਧਾਰ_Font_Name:
	dictval.nameSIDs[name_dict_ਮੁੱਲਾਂ_t::name_op_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (op)] = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_is_Fixed_Pitch:
      ਹਾਲਤ Op_ਕੋਡ_Italic_Angle:
      ਹਾਲਤ Op_ਕੋਡ_Underline_Position:
      ਹਾਲਤ Op_ਕੋਡ_Underline_Thickness:
      ਹਾਲਤ Op_ਕੋਡ_Paint_Type:
      ਹਾਲਤ Op_ਕੋਡ_Charstring_Type:
      ਹਾਲਤ Op_ਕੋਡ_Unique_ID:
      ਹਾਲਤ Op_ਕੋਡ_Stroke_ਚੌੜਾਈ:
      ਹਾਲਤ Op_ਕੋਡ_Synthetic_ਅਧਾਰ:
      ਹਾਲਤ Op_ਕੋਡ_CIDFont_ਸੰਸਕਰਣ:
      ਹਾਲਤ Op_ਕੋਡ_CIDFont_Revision:
      ਹਾਲਤ Op_ਕੋਡ_CIDFont_Type:
      ਹਾਲਤ Op_ਕੋਡ_UIDBase:
      ਹਾਲਤ Op_ਕੋਡ_Font_BBox:
      ਹਾਲਤ Op_ਕੋਡ_XUID:
      ਹਾਲਤ Op_ਕੋਡ_ਅਧਾਰ_Font_Blend:
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_CIDCount:
	dictval.cid_ਗਿਣਤੀ = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_ROS:
	dictval.ros_supplement = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	dictval.nameSIDs[name_dict_ਮੁੱਲਾਂ_t::ordering] = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	dictval.nameSIDs[name_dict_ਮੁੱਲਾਂ_t::registry] = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	env.clear_ਦਲੀਲਾਂ ();
	ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;
	ਪ੍ਰਤਿਜ੍ਞਾ (env.ਆਖਰੀ_ਵਿੱਥ > dictval.op_ਸ਼ੁਰੂ);
	val.ਆਖਰੀ_arg_ਵਿੱਥ = (env.ਆਖਰੀ_ਵਿੱਥ - 1) - dictval.op_ਸ਼ੁਰੂ;  /* offset to the last argument */
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_Encoding:
	dictval.Encoding_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਜੇ (unlikely (dictval.Encoding_ਵਿੱਥ == 0)) ਵਾਪਸ;
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_charset:
	dictval.Charset_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਜੇ (unlikely (dictval.Charset_ਵਿੱਥ == 0)) ਵਾਪਸ;
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_FDSelect:
	dictval.FDSelect_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_ਨਿੱਜੀ:
	dictval.ਨਿੱਜੀ_Dict_Info.ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	dictval.ਨਿੱਜੀ_Dict_Info.ਮਾਪ = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਮੂਲ:
	env.ਆਖਰੀ_ਵਿੱਥ = env.str_ref.get_ਵਿੱਥ ();
	ਸਿਖਰ_dict_opset_t<cff1_ਸਿਖਰ_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
	ਰੋਕੋ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    dictval.ਜੋੜੋ_op (op, env.str_ref, val);
  }
};

ਬਣਤਰ cff1_font_dict_ਮੁੱਲਾਂ_t : dict_ਮੁੱਲਾਂ_t<op_str_t>
{
  ਖਾਲੀ init ()
  {
    dict_ਮੁੱਲਾਂ_t<op_str_t>::init ();
    ਨਿੱਜੀ_Dict_Info.init ();
    fontName = CFF_UNDEF_SID;
  }
  ਖਾਲੀ fini () { dict_ਮੁੱਲਾਂ_t<op_str_t>::fini (); }

  ਸਾਰਣੀ_info_t       ਨਿੱਜੀ_Dict_Info;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    fontName;
};

ਬਣਤਰ cff1_font_dict_opset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, ਅੰਕ_interp_env_t& env, cff1_font_dict_ਮੁੱਲਾਂ_t& dictval)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_Font_Name:
	dictval.fontName = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_Font_ਮੈਟ੍ਰਿਕਸ:
      ਹਾਲਤ Op_ਕੋਡ_Paint_Type:
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_ਨਿੱਜੀ:
	dictval.ਨਿੱਜੀ_Dict_Info.ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	dictval.ਨਿੱਜੀ_Dict_Info.ਮਾਪ = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਮੂਲ:
	dict_opset_t::process_op (op, env);
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
	ਰੋਕੋ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    dictval.ਜੋੜੋ_op (op, env.str_ref);
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VAL>
ਬਣਤਰ cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_ਅਧਾਰ_t : dict_ਮੁੱਲਾਂ_t<VAL>
{
  ਖਾਲੀ init ()
  {
    dict_ਮੁੱਲਾਂ_t<VAL>::init ();
    subrs_ਵਿੱਥ = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  ਖਾਲੀ fini () { dict_ਮੁੱਲਾਂ_t<VAL>::fini (); }

  ਪੂਰਨ                 subrs_ਵਿੱਥ;
  ਅਚਲ CFF1Subrs    *localSubrs;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_ਅਧਾਰ_t<op_str_t> cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_subset_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_ਅਧਾਰ_t<ਅੰਕ_dict_val_t> cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_t;

ਬਣਤਰ cff1_ਨਿੱਜੀ_dict_opset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, ਅੰਕ_interp_env_t& env, cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_t& dictval)
  {
    ਅੰਕ_dict_val_t val;
    val.init ();

    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਮੁੱਲਾਂ:
      ਹਾਲਤ Op_ਕੋਡ_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_H:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_V:
      ਹਾਲਤ Op_ਕੋਡ_Std_HW:
      ਹਾਲਤ Op_ਕੋਡ_Std_VW:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Scale:
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਖਿਸਕਾਓ:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Fuzz:
      ਹਾਲਤ Op_ਕੋਡ_Force_Bold:
      ਹਾਲਤ Op_ਕੋਡ_Language_ਸਮੂਹ:
      ਹਾਲਤ Op_ਕੋਡ_Expansion_ਗੁਣਕ:
      ਹਾਲਤ Op_ਕੋਡ_initial_Random_ਬੀਜ:
      ਹਾਲਤ Op_ਕੋਡ_default_ਚੌੜਾਈ_X:
      ਹਾਲਤ Op_ਕੋਡ_nominal_ਚੌੜਾਈ_X:
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_Subrs:
	dictval.subrs_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਮੂਲ:
	dict_opset_t::process_op (op, env);
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
	ਰੋਕੋ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    dictval.ਜੋੜੋ_op (op, env.str_ref, val);
  }
};

ਬਣਤਰ cff1_ਨਿੱਜੀ_dict_opset_subset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, ਅੰਕ_interp_env_t& env, cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_subset_t& dictval)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਮੁੱਲਾਂ:
      ਹਾਲਤ Op_ਕੋਡ_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_H:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_V:
      ਹਾਲਤ Op_ਕੋਡ_Std_HW:
      ਹਾਲਤ Op_ਕੋਡ_Std_VW:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Scale:
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਖਿਸਕਾਓ:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Fuzz:
      ਹਾਲਤ Op_ਕੋਡ_Force_Bold:
      ਹਾਲਤ Op_ਕੋਡ_Language_ਸਮੂਹ:
      ਹਾਲਤ Op_ਕੋਡ_Expansion_ਗੁਣਕ:
      ਹਾਲਤ Op_ਕੋਡ_initial_Random_ਬੀਜ:
      ਹਾਲਤ Op_ਕੋਡ_default_ਚੌੜਾਈ_X:
      ਹਾਲਤ Op_ਕੋਡ_nominal_ਚੌੜਾਈ_X:
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_Subrs:
	dictval.subrs_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਮੂਲ:
	dict_opset_t::process_op (op, env);
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
	ਰੋਕੋ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    dictval.ਜੋੜੋ_op (op, env.str_ref);
  }
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ dict_interpreter_t<cff1_ਸਿਖਰ_dict_opset_t, cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t, cff1_ਸਿਖਰ_dict_interp_env_t> cff1_ਸਿਖਰ_dict_interpreter_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_ਮੁੱਲਾਂ_t> cff1_font_dict_interpreter_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ CFF1_ਸੂਚਕਾਂਕ CFF1_Name_ਸੂਚਕਾਂਕ;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ CFF1_ਸੂਚਕਾਂਕ CFF1_ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ;

ਬਣਤਰ cff1_font_dict_ਮੁੱਲਾਂ_mod_t
{
  cff1_font_dict_ਮੁੱਲਾਂ_mod_t() { init (); }

  ਖਾਲੀ init () { init ( &Null (cff1_font_dict_ਮੁੱਲਾਂ_t), CFF_UNDEF_SID ); }

  ਖਾਲੀ init (ਅਚਲ cff1_font_dict_ਮੁੱਲਾਂ_t *ਅਧਾਰ_2,
	     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fontName_)
  {
    ਅਧਾਰ = ਅਧਾਰ_2;
    fontName = fontName_;
    ਨਿੱਜੀ_Dict_Info.init ();
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ ਅਧਾਰ->get_ਗਿਣਤੀ (); }

  ਅਚਲ op_str_t &ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ { ਵਾਪਸ (*ਅਧਾਰ)[i]; }

  ਅਚਲ cff1_font_dict_ਮੁੱਲਾਂ_t    *ਅਧਾਰ;
  ਸਾਰਣੀ_info_t		   ਨਿੱਜੀ_Dict_Info;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		fontName;
};

ਬਣਤਰ CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ITER, ਕਿਸਮ_ਨਾਂ OP_SERIALIZER>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ਵਾਪਸ FDArray<HBUINT16>::serialize<cff1_font_dict_ਮੁੱਲਾਂ_mod_t, cff1_font_dict_ਮੁੱਲਾਂ_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

ਨਾਮਾਕਾਸ਼ OT {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ CFF;

ਬਣਤਰ cff1
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_CFF1;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ PRIVOPSET, ਕਿਸਮ_ਨਾਂ PRIVDICTVAL>
  ਬਣਤਰ accelerator_templ_t
  {
    ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = cff1::ਸਾਰਣੀ_ਟੈਗ;

    accelerator_templ_t (hb_face_t *face)
    {
      ਜੇ (!face) ਵਾਪਸ;

      ਸਿਖਰ_Dict_2.init ();
      fontDicts.init ();
      ਨਿੱਜੀ_Dicts.init ();

      ਇਹ->blob = sc.reference_ਸਾਰਣੀ<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (ਇਹ->blob);
      sc.ਸ਼ੁਰੂ_processing ();

      ਅਚਲ OT::cff1 *cff = ਇਹ->blob->ਸਾਂਚਾ as<OT::cff1> ();

      ਜੇ (cff == &Null (OT::cff1))
        ਜਾਓ fail;

      name_ਸੂਚਕਾਂਕ = &cff->name_ਸੂਚਕਾਂਕ (cff);
      ਜੇ ((name_ਸੂਚਕਾਂਕ == &Null (CFF1_Name_ਸੂਚਕਾਂਕ)) || !name_ਸੂਚਕਾਂਕ->sanitize (&sc))
        ਜਾਓ fail;
      hb_barrier ();

      ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ = &Struct_At_ਵਿੱਥ_Or_Null<CFF1_ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ> (name_ਸੂਚਕਾਂਕ, name_ਸੂਚਕਾਂਕ->get_ਮਾਪ (), sc);
      ਜੇ (ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ == &Null (CFF1_ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ) || (ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ->ਗਿਣਤੀ_2 == 0))
        ਜਾਓ fail;
      hb_barrier ();

      { /* parse top dict */
	ਅਚਲ hb_ubytes_t ਸਿਖਰ_Dict_Str = (*ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ)[0];
	ਜੇ (unlikely (!ਸਿਖਰ_Dict_Str.sanitize (&sc)))   ਜਾਓ fail;
	hb_barrier ();
	cff1_ਸਿਖਰ_dict_interp_env_t env (ਸਿਖਰ_Dict_Str);
	cff1_ਸਿਖਰ_dict_interpreter_t ਸਿਖਰ_interp (env);
	ਜੇ (unlikely (!ਸਿਖਰ_interp.interpret (ਸਿਖਰ_Dict_2)))   ਜਾਓ fail;
      }

      ਜੇ (is_predef_charset ())
	charset = &Null (Charset);
      ਨਹੀਂਤਾਂ
      {
	charset = &Struct_At_ਵਿੱਥ_Or_Null<Charset> (cff, ਸਿਖਰ_Dict_2.Charset_ਵਿੱਥ, sc, &ਅੰਕ_charset_entries);
	ਜੇ (unlikely (charset == &Null (Charset)))   ਜਾਓ fail;
      }

      fd_ਗਿਣਤੀ = 1;
      ਜੇ (is_CID ())
      {
	fdArray = &Struct_At_ਵਿੱਥ_Or_Null<CFF1FDArray> (cff, ਸਿਖਰ_Dict_2.FDArray_ਵਿੱਥ, sc);
	fdSelect = &Struct_At_ਵਿੱਥ_Or_Null<CFF1FDSelect> (cff, ਸਿਖਰ_Dict_2.FDSelect_ਵਿੱਥ, sc, fdArray->ਗਿਣਤੀ_2);
	ਜੇ (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  ਜਾਓ fail;

	fd_ਗਿਣਤੀ = fdArray->ਗਿਣਤੀ_2;
      }
      ਨਹੀਂਤਾਂ
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      ਜੇ (is_CID ())
      {
	ਜੇ (unlikely (charset == &Null (Charset)))   ਜਾਓ fail;
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (!is_predef_encoding ())
	{
	  encoding = &Struct_At_ਵਿੱਥ_Or_Null<Encoding> (cff, ਸਿਖਰ_Dict_2.Encoding_ਵਿੱਥ, sc);
	  ਜੇ (unlikely (encoding == &Null (Encoding)))   ਜਾਓ fail;
	}
      }

      string_ਸੂਚਕਾਂਕ = &Struct_At_ਵਿੱਥ_Or_Null<CFF1_String_ਸੂਚਕਾਂਕ> (ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ, ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ->get_ਮਾਪ (), sc);
      ਜੇ (string_ਸੂਚਕਾਂਕ == &Null (CFF1_String_ਸੂਚਕਾਂਕ))
        ਜਾਓ fail;

      globalSubrs = &Struct_At_ਵਿੱਥ_Or_Null<CFF1Subrs> (string_ਸੂਚਕਾਂਕ, string_ਸੂਚਕਾਂਕ->get_ਮਾਪ (), sc);
      char_ਲੜੀਆਂ = &Struct_At_ਵਿੱਥ_Or_Null<CFF1_Char_ਲੜੀਆਂ> (cff, ਸਿਖਰ_Dict_2.char_ਲੜੀਆਂ_ਵਿੱਥ, sc);
      ਜੇ (char_ਲੜੀਆਂ == &Null (CFF1_Char_ਲੜੀਆਂ))
        ਜਾਓ fail;

      ਅੰਕ_glyphs = char_ਲੜੀਆਂ->ਗਿਣਤੀ_2;
      ਜੇ (ਅੰਕ_glyphs != sc.get_ਅੰਕ_glyphs ())
        ਜਾਓ fail;

      ਜੇ (unlikely (!ਨਿੱਜੀ_Dicts.resize (fd_ਗਿਣਤੀ)))
        ਜਾਓ fail;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < fd_ਗਿਣਤੀ; i++)
	ਨਿੱਜੀ_Dicts[i].init ();

      // parse CID font dicts and gather private dicts
      ਜੇ (is_CID ())
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < fd_ਗਿਣਤੀ; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  ਜੇ (unlikely (!fontDictStr.sanitize (&sc)))   ਜਾਓ fail;
	  hb_barrier ();
	  cff1_font_dict_ਮੁੱਲਾਂ_t *font;
	  cff1_ਸਿਖਰ_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  ਜੇ (unlikely (fontDicts.ਵਿੱਚ_error ()))   ਜਾਓ fail;

	  font->init ();
	  ਜੇ (unlikely (!font_interp.interpret (*font)))   ਜਾਓ fail;
	  PRIVDICTVAL *priv = &ਨਿੱਜੀ_Dicts[i];
	  ਅਚਲ hb_ubytes_t privDictStr = Struct_At_ਵਿੱਥ_Or_Null<UnsizedByteStr> (cff, font->ਨਿੱਜੀ_Dict_Info.ਵਿੱਥ, sc, font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ).as_ubytes (font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ);
	  ਜੇ (unlikely (font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ &&
			privDictStr == (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &Null (UnsizedByteStr))) ਜਾਓ fail;
	  ਅੰਕ_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  ਜੇ (unlikely (!priv_interp.interpret (*priv)))   ਜਾਓ fail;

	  priv->localSubrs = &Struct_At_ਵਿੱਥ_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_ਵਿੱਥ, sc);
	}
      }
      ਨਹੀਂਤਾਂ  /* non-CID */
      {
	cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t *font = &ਸਿਖਰ_Dict_2;
	PRIVDICTVAL *priv = &ਨਿੱਜੀ_Dicts[0];

	ਅਚਲ hb_ubytes_t privDictStr = Struct_At_ਵਿੱਥ_Or_Null<UnsizedByteStr> (cff, font->ਨਿੱਜੀ_Dict_Info.ਵਿੱਥ, sc, font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ).as_ubytes (font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ);
	ਜੇ (font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ &&
	    unlikely (privDictStr == (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &Null (UnsizedByteStr))) ਜਾਓ fail;
	ਅੰਕ_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	ਜੇ (unlikely (!priv_interp.interpret (*priv)))   ਜਾਓ fail;

	priv->localSubrs = &Struct_At_ਵਿੱਥ_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_ਵਿੱਥ, sc);
	hb_barrier ();
      }

      ਵਾਪਸ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    ਖਾਲੀ _fini ()
    {
      sc.ਸਿਰਾ_processing ();
      ਸਿਖਰ_Dict_2.fini ();
      fontDicts.fini ();
      ਨਿੱਜੀ_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = ਖਾਲੀ_ਸੂਚਕ;
    }

    hb_blob_t *get_blob () ਅਚਲ { ਵਾਪਸ blob; }

    ਤਾਰਕਿਕ is_valid () ਅਚਲ { ਵਾਪਸ blob; }
    ਤਾਰਕਿਕ   is_CID () ਅਚਲ { ਵਾਪਸ ਸਿਖਰ_Dict_2.is_CID (); }

    ਤਾਰਕਿਕ is_predef_charset () ਅਚਲ { ਵਾਪਸ ਸਿਖਰ_Dict_2.Charset_ਵਿੱਥ <= ExpertSubsetCharset; }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ std_ਕੋਡ_ਮੰਜ਼ਿਲ_glyph (hb_codepoint_t ਕੋਡ) ਅਚਲ
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (ਕੋਡ);
      ਜੇ (unlikely (sid == CFF_UNDEF_SID))
	ਵਾਪਸ 0;

      ਜੇ (charset != &Null (Charset))
	ਵਾਪਸ charset->get_glyph (sid, ਅੰਕ_glyphs);
      ਨਹੀਂਤਾਂ ਜੇ ((ਸਿਖਰ_Dict_2.Charset_ਵਿੱਥ == ISOAdobeCharset)
	      && (ਕੋਡ <= 228 /*zcaron*/)) ਵਾਪਸ sid;
      ਵਾਪਸ 0;
    }

    ਤਾਰਕਿਕ is_predef_encoding () ਅਚਲ { ਵਾਪਸ ਸਿਖਰ_Dict_2.Encoding_ਵਿੱਥ <= ExpertEncoding; }

    hb_codepoint_t glyph_ਮੰਜ਼ਿਲ_ਕੋਡ (hb_codepoint_t glyph,
				  ਕੋਡ_pair_t *glyph_ਮੰਜ਼ਿਲ_sid_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
    {
      ਜੇ (encoding != &Null (Encoding))
	ਵਾਪਸ encoding->get_ਕੋਡ (glyph);
      ਨਹੀਂਤਾਂ
      {
	hb_codepoint_t sid = glyph_ਮੰਜ਼ਿਲ_sid (glyph, glyph_ਮੰਜ਼ਿਲ_sid_cache);
	ਜੇ (sid == 0) ਵਾਪਸ 0;
	hb_codepoint_t ਕੋਡ = 0;
	ਚੋਣ (ਸਿਖਰ_Dict_2.Encoding_ਵਿੱਥ)
	{
	ਹਾਲਤ StandardEncoding:
	  ਕੋਡ = lookup_standard_encoding_for_ਕੋਡ (sid);
	  ਰੋਕੋ;
	ਹਾਲਤ ExpertEncoding:
	  ਕੋਡ = lookup_expert_encoding_for_ਕੋਡ (sid);
	  ਰੋਕੋ;
	ਮੂਲ:
	  ਰੋਕੋ;
	}
	ਵਾਪਸ ਕੋਡ;
      }
    }

    glyph_ਮੰਜ਼ਿਲ_sid_map_t *create_glyph_ਮੰਜ਼ਿਲ_sid_map () ਅਚਲ
    {
      ਜੇ (charset != &Null (Charset))
      {
	ਸਵੈਚਾਲਿਤ *mapping = (glyph_ਮੰਜ਼ਿਲ_sid_map_t *) hb_malloc (ਆਕਾਰ (glyph_ਮੰਜ਼ਿਲ_sid_map_t));
	ਜੇ (unlikely (!mapping)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
	mapping = ਨਵਾਂ (mapping) glyph_ਮੰਜ਼ਿਲ_sid_map_t ();
	mapping->push (ਕੋਡ_pair_t {0, 1});
	charset->collect_glyph_ਮੰਜ਼ਿਲ_sid_map (mapping, ਅੰਕ_glyphs);
	ਵਾਪਸ mapping;
      }
      ਨਹੀਂਤਾਂ
	ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }

    hb_codepoint_t glyph_ਮੰਜ਼ਿਲ_sid (hb_codepoint_t glyph,
				 ਕੋਡ_pair_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
    {
      ਜੇ (charset != &Null (Charset))
	ਵਾਪਸ charset->get_sid (glyph, ਅੰਕ_glyphs, cache);
      ਨਹੀਂਤਾਂ
      {
	hb_codepoint_t sid = 0;
	ਚੋਣ (ਸਿਖਰ_Dict_2.Charset_ਵਿੱਥ)
	{
	  ਹਾਲਤ ISOAdobeCharset:
	    ਜੇ (glyph <= 228 /*zcaron*/) sid = glyph;
	    ਰੋਕੋ;
	  ਹਾਲਤ ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    ਰੋਕੋ;
	  ਹਾਲਤ ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    ਰੋਕੋ;
	  ਮੂਲ:
	    ਰੋਕੋ;
	}
	ਵਾਪਸ sid;
      }
    }

    hb_codepoint_t sid_ਮੰਜ਼ਿਲ_glyph (hb_codepoint_t sid) ਅਚਲ
    {
      ਜੇ (charset != &Null (Charset))
	ਵਾਪਸ charset->get_glyph (sid, ਅੰਕ_glyphs);
      ਨਹੀਂਤਾਂ
      {
	hb_codepoint_t glyph = 0;
	ਚੋਣ (ਸਿਖਰ_Dict_2.Charset_ਵਿੱਥ)
	{
	  ਹਾਲਤ ISOAdobeCharset:
	    ਜੇ (sid <= 228 /*zcaron*/) glyph = sid;
	    ਰੋਕੋ;
	  ਹਾਲਤ ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    ਰੋਕੋ;
	  ਹਾਲਤ ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    ਰੋਕੋ;
	  ਮੂਲ:
	    ਰੋਕੋ;
	}
	ਵਾਪਸ glyph;
      }
    }

    ਸੁਰੱਖਿਅਤ:
    hb_sanitize_context_t   sc;

    ਜਨਤਕ:
    hb_blob_t               *blob = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ Encoding	    *encoding = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ Charset	    *charset = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF1_Name_ਸੂਚਕਾਂਕ     *name_ਸੂਚਕਾਂਕ = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF1_ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ  *ਸਿਖਰ_Dict_ਸੂਚਕਾਂਕ = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF1_String_ਸੂਚਕਾਂਕ   *string_ਸੂਚਕਾਂਕ = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF1Subrs	    *globalSubrs = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF1_Char_ਲੜੀਆਂ   *char_ਲੜੀਆਂ = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF1FDArray       *fdArray = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF1FDSelect      *fdSelect = ਖਾਲੀ_ਸੂਚਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	     fd_ਗਿਣਤੀ = 0;

    cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t   ਸਿਖਰ_Dict_2;
    hb_ਸਦਿਸ਼_t<cff1_font_dict_ਮੁੱਲਾਂ_t>
			     fontDicts;
    hb_ਸਦਿਸ਼_t<PRIVDICTVAL> ਨਿੱਜੀ_Dicts;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	     ਅੰਕ_glyphs = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	     ਅੰਕ_charset_entries = 0;
  };

  ਬਣਤਰ accelerator_t : accelerator_templ_t<cff1_ਨਿੱਜੀ_dict_opset_t, cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_ਨਾਂ.ਸੈੱਟ_relaxed (ਖਾਲੀ_ਸੂਚਕ);

      ਜੇ (!is_valid ()) ਵਾਪਸ;
      ਜੇ (is_CID ()) ਵਾਪਸ;
    }
    ~accelerator_t ()
    {
      hb_sorted_ਸਦਿਸ਼_t<gname_t> *ਨਾਂ = glyph_ਨਾਂ.get_relaxed ();
      ਜੇ (ਨਾਂ)
      {
	ਨਾਂ->fini ();
	hb_free (ਨਾਂ);
      }
    }

    ਤਾਰਕਿਕ get_glyph_name (hb_codepoint_t glyph,
			 ਅੱਖਰ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_len) ਅਚਲ
    {
      ਜੇ (unlikely (glyph >= ਅੰਕ_glyphs)) ਵਾਪਸ ਝੂਠ;
      ਜੇ (unlikely (!is_valid ())) ਵਾਪਸ ਝੂਠ;
      ਜੇ (is_CID()) ਵਾਪਸ ਝੂਠ;
      ਜੇ (unlikely (!buf_len)) ਵਾਪਸ ਸੱਚ;
      hb_codepoint_t sid = glyph_ਮੰਜ਼ਿਲ_sid (glyph);
      ਅਚਲ ਅੱਖਰ *str;
      size_t str_len;
      ਜੇ (sid < cff1_std_ਲੜੀਆਂ_ਲੰਮਾਈ)
      {
	hb_ਬਾਈਟਾਂ_t byte_str = cff1_std_ਲੜੀਆਂ (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.ਲੰਮਾਈ;
      }
      ਨਹੀਂਤਾਂ
      {
	hb_ubytes_t ubyte_str = (*string_ਸੂਚਕਾਂਕ)[sid - cff1_std_ਲੜੀਆਂ_ਲੰਮਾਈ];
	str = (ਅਚਲ ਅੱਖਰ *)ubyte_str.arrayZ;
	str_len = ubyte_str.ਲੰਮਾਈ;
      }
      ਜੇ (!str_len) ਵਾਪਸ ਝੂਠ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ਅਚਲ ਅੱਖਰ*)str, len);
      buf[len] = '\0';
      ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ get_glyph_from_name (ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
			      hb_codepoint_t *glyph) ਅਚਲ
    {
      ਜੇ (unlikely (!is_valid ())) ਵਾਪਸ ਝੂਠ;
      ਜੇ (is_CID()) ਵਾਪਸ ਝੂਠ;
      ਜੇ (len < 0) len = ਲੜੀ_ਲੰਬਾਈ (name);
      ਜੇ (unlikely (!len)) ਵਾਪਸ ਝੂਠ;

    retry:
      hb_sorted_ਸਦਿਸ਼_t<gname_t> *ਨਾਂ = glyph_ਨਾਂ.get_acquire ();
      ਜੇ (unlikely (!ਨਾਂ))
      {
	ਨਾਂ = (hb_sorted_ਸਦਿਸ਼_t<gname_t> *) hb_calloc (1, ਆਕਾਰ (hb_sorted_ਸਦਿਸ਼_t<gname_t>));
	ਜੇ (likely (ਨਾਂ))
	{
	  ਨਾਂ->init ();
	  /* TODO */

	  /* fill glyph names */
	  ਕੋਡ_pair_t glyph_ਮੰਜ਼ਿਲ_sid_cache {0, HB_CODEPOINT_INVALID};
	  ਲਈ (hb_codepoint_t gid = 0; gid < ਅੰਕ_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_ਮੰਜ਼ਿਲ_sid (gid, &glyph_ਮੰਜ਼ਿਲ_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    ਜੇ (sid < cff1_std_ਲੜੀਆਂ_ਲੰਮਾਈ)
	      gname.name = cff1_std_ਲੜੀਆਂ (sid);
	    ਨਹੀਂਤਾਂ
	    {
	      hb_ubytes_t	ustr = (*string_ਸੂਚਕਾਂਕ)[sid - cff1_std_ਲੜੀਆਂ_ਲੰਮਾਈ];
	      gname.name = hb_ਬਾਈਟਾਂ_t ((ਅਚਲ ਅੱਖਰ*) ustr.arrayZ, ustr.ਲੰਮਾਈ);
	    }
	    ਜੇ (unlikely (!gname.name.arrayZ))
	      gname.name = hb_ਬਾਈਟਾਂ_t ("", 0); /* To avoid nullptr. */
	    ਨਾਂ->push (gname);
	  }
	  ਨਾਂ->qsort ();
	}
	ਜੇ (unlikely (!glyph_ਨਾਂ.cmpexch (ਖਾਲੀ_ਸੂਚਕ, ਨਾਂ)))
	{
	  ਜੇ (ਨਾਂ)
	  {
	    ਨਾਂ->fini ();
	    hb_free (ਨਾਂ);
	  }
	  ਜਾਓ retry;
	}
      }

      gname_t key = { hb_ਬਾਈਟਾਂ_t (name, len), 0 };
      ਅਚਲ gname_t *gname = ਨਾਂ ? ਨਾਂ->bsearch (key) : ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (!gname) ਵਾਪਸ ਝੂਠ;
      hb_codepoint_t gid = sid_ਮੰਜ਼ਿਲ_glyph (gname->sid);
      ਜੇ (!gid && gname->sid) ਵਾਪਸ ਝੂਠ;
      *glyph = gid;
      ਵਾਪਸ ਸੱਚ;
    }

    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ;
    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ get_ਰਸਤਾ (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ;

    ਨਿਜੀ:
    ਬਣਤਰ gname_t
    {
      hb_ਬਾਈਟਾਂ_t	name;
      uint16_t		sid;

      ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
      {
	ਅਚਲ gname_t *a = (ਅਚਲ gname_t *)a_;
	ਅਚਲ gname_t *b = (ਅਚਲ gname_t *)b_;
	ਬਿਨਾਨਿਸ਼ਾਨੀ minlen = hb_min (a->name.ਲੰਮਾਈ, b->name.ਲੰਮਾਈ);
	ਪੂਰਨ ret = ਲੜੀ_ਤੁਲਨਾ (a->name.arrayZ, b->name.arrayZ, minlen);
	ਜੇ (ret) ਵਾਪਸ ret;
	ਵਾਪਸ a->name.ਲੰਮਾਈ - b->name.ਲੰਮਾਈ;
      }

      ਪੂਰਨ cmp (ਅਚਲ gname_t &a) ਅਚਲ { ਵਾਪਸ cmp (&a, ਇਹ); }
    };

    ਬਦਲਣਯੋਗ hb_atomic_t<hb_sorted_ਸਦਿਸ਼_t<gname_t> *> glyph_ਨਾਂ;

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ accelerator_templ_t<cff1_ਨਿੱਜੀ_dict_opset_t, cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_t> SUPER;
  };

  ਬਣਤਰ accelerator_subset_t : accelerator_templ_t<cff1_ਨਿੱਜੀ_dict_opset_subset_t, cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ਜੇ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ;
    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
				ਬਣਤਰ cff1_subset_plan &plan) ਅਚਲ;
    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *ਅਧਾਰ, hb_codepoint_t *accent) ਅਚਲ;

    ਬਦਲਣਯੋਗ CFF::cff_subset_accelerator_t* cff_accelerator = ਖਾਲੀ_ਸੂਚਕ;

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ accelerator_templ_t<cff1_ਨਿੱਜੀ_dict_opset_subset_t, cff1_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_subset_t> SUPER;
  };

  ਸੁਰੱਖਿਅਤ:
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ hb_codepoint_t lookup_standard_encoding_for_ਕੋਡ (hb_codepoint_t sid);
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ hb_codepoint_t lookup_expert_encoding_for_ਕੋਡ (hb_codepoint_t sid);
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t ਕੋਡ);

  ਜਨਤਕ:
  Fixed_ਸੰਸਕਰਣ<HBUINT8> ਸੰਸਕਰਣ;	  /* Version of CFF table. set to 0x0100u */
  NNOffset_ਮੰਜ਼ਿਲ<CFF1_Name_ਸੂਚਕਾਂਕ, HBUINT8> name_ਸੂਚਕਾਂਕ; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_ਮਾਪ;	  /* offset size (unused?) */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

ਬਣਤਰ cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *builder) ਅਚਲ
  {
    ਜੇ (!is_valid ()) ਵਾਪਸ;
    ਲਈ (hb_codepoint_t gid = 0; gid < ਅੰਕ_glyphs; gid++)
    {
      hb_codepoint_t ਅਧਾਰ_gid, accent_gid;
      ਜੇ (get_seac_components (gid, &ਅਧਾਰ_gid, &accent_gid))
      {
        builder->ਜੋੜੋ_depend (gid, HB_ਟੈਗ('C','F','F',' '), ਅਧਾਰ_gid);
        builder->ਜੋੜੋ_depend (gid, HB_ਟੈਗ('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#ਜੇ_ਅੰਤ /* HB_OT_CFF1_TABLE_HH */
