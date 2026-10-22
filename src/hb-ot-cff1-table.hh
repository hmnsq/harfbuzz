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

#నిర్వచితం_కాకపోతే HB_OT_CFF1_పట్టిక_HH
#నిర్వచించు HB_OT_CFF1_పట్టిక_HH

#అన్తర్భావ "hb-ot-cff-common.hh"
#అన్తర్భావ "hb-depend-data.hh"
#అన్తర్భావ "hb-subset-cff-common.hh"
#అన్తర్భావ "hb-draw.hh"
#అన్తర్భావ "hb-paint.hh"

#నిర్వచించు HB_STRING_ARRAY_NAME cff1_std_అక్షరశ్రేణులు
#నిర్వచించు HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#అన్తర్భావ "hb-string-array.hh"
#నిర్వచనం_రద్దు HB_STRING_ARRAY_LIST
#నిర్వచనం_రద్దు HB_STRING_ARRAY_NAME

నామాకాశ CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#నిర్వచించు HB_OT_ట్యాగ్_CFF1 HB_ట్యాగ్('C','F','F',' ')

#నిర్వచించు CFF_UNDEF_SID   CFF_UNDEF_సంకేతం

గణన EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
గణన CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

రకనిర్వచనం CFF1_సూచీ          CFF1_Char_అక్షరశ్రేణులు;
రకనిర్వచనం Subrs<HBUINT16>    CFF1Subrs;

నిర్మాణం CFF1FDSelect : FDSelect {};

/* Encoding */
నిర్మాణం Encoding0 {
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_సంకేతం (hb_codepoint_t glyph) స్థిరరాశి
  {
    ప్రతిజ్ఞా (glyph > 0);
    glyph--;
    ఒకవేళ (glyph < nCodes ())
    {
      తిరిగి (hb_codepoint_t)codes[glyph];
    }
    లేకపోతే
      తిరిగి CFF_UNDEF_సంకేతం;
  }

  HBUINT8 &nCodes () { తిరిగి codes.len; }
  HBUINT8 nCodes () స్థిరరాశి { తిరిగి codes.len; }

  Array_యొక్క<HBUINT8, HBUINT8> codes;

  DEFINE_కొలత_ARRAY_SIZED (1, codes);
};

నిర్మాణం Encoding1_Range {
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT8   మొదటి;
  HBUINT8   n_ఎడమ;

  DEFINE_కొలత_STATIC (2);
};

నిర్మాణం Encoding1 {
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_సంకేతం (hb_codepoint_t glyph) స్థిరరాశి
  {
    /* TODO: Add cache like get_sid. */
    ప్రతిజ్ఞా (glyph > 0);
    glyph--;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < nRanges (); i++)
    {
      ఒకవేళ (glyph <= ranges[i].n_ఎడమ)
      {
	hb_codepoint_t సంకేతం_2 = (hb_codepoint_t) ranges[i].మొదటి + glyph;
	తిరిగి (likely (సంకేతం_2 < 0x100) ? సంకేతం_2: CFF_UNDEF_సంకేతం);
      }
      glyph -= (ranges[i].n_ఎడమ + 1);
    }
    తిరిగి CFF_UNDEF_సంకేతం;
  }

  HBUINT8 &nRanges () { తిరిగి ranges.len; }
  HBUINT8 nRanges () స్థిరరాశి { తిరిగి ranges.len; }

  Array_యొక్క<Encoding1_Range, HBUINT8> ranges;

  DEFINE_కొలత_ARRAY_SIZED (1, ranges);
};

నిర్మాణం SuppEncoding {
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT8   సంకేతం_2;
  HBUINT16  glyph;

  DEFINE_కొలత_STATIC (3);
};

నిర్మాణం CFF1_Supp_Enc_దత్తాంశం {
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (supps.sanitize (c));
  }

  శూన్య get_codes (hb_codepoint_t sid, hb_సదిశ_t<hb_codepoint_t> &codes) స్థిరరాశి
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < nSups (); i++)
      ఒకవేళ (sid == supps[i].glyph)
	codes.push (supps[i].సంకేతం_2);
  }

  HBUINT8 &nSups () { తిరిగి supps.len; }
  HBUINT8 nSups () స్థిరరాశి { తిరిగి supps.len; }

  Array_యొక్క<SuppEncoding, HBUINT8> supps;

  DEFINE_కొలత_ARRAY_SIZED (1, supps);
};

నిర్మాణం Encoding
{
  /* serialize a fullset Encoding */
  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి Encoding &src)
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  తార్కిక serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  గుర్తులేని పూర్ణాంకం enc_లెక్క,
		  స్థిరరాశి hb_సదిశ_t<సంకేతం_pair_t>& సంకేతం_ranges,
		  స్థిరరాశి hb_సదిశ_t<సంకేతం_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (ఇది);
    Encoding *dest = c->extend_min (ఇది);
    ఒకవేళ (unlikely (!dest)) return_trace (అబద్ధం);
    dest->format = format | ((supp_codes.నిడివి > 0) ? 0x80 : 0);
    ఎంపిక (format) {
    స్థితి 0:
    {
      Encoding0 *fmt0 = c->allocate_కొలత<Encoding0> (Encoding0::min_కొలత + HBUINT8::static_కొలత * enc_లెక్క);
      ఒకవేళ (unlikely (!fmt0)) return_trace (అబద్ధం);
      fmt0->nCodes () = enc_లెక్క;
      గుర్తులేని పూర్ణాంకం glyph = 0;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < సంకేతం_ranges.నిడివి; i++)
      {
	hb_codepoint_t సంకేతం_2 = సంకేతం_ranges[i].సంకేతం_2;
	కొరకు (పూర్ణాంకం ఎడమ = (పూర్ణాంకం)సంకేతం_ranges[i].glyph; ఎడమ >= 0; ఎడమ--)
	  fmt0->codes[glyph++] = సంకేతం_2++;
	ఒకవేళ (unlikely (!((glyph <= 0x100) && (సంకేతం_2 <= 0x100))))
	  return_trace (అబద్ధం);
      }
    }
    ఆపు;

    స్థితి 1:
    {
      Encoding1 *fmt1 = c->allocate_కొలత<Encoding1> (Encoding1::min_కొలత + Encoding1_Range::static_కొలత * సంకేతం_ranges.నిడివి);
      ఒకవేళ (unlikely (!fmt1)) return_trace (అబద్ధం);
      fmt1->nRanges () = సంకేతం_ranges.నిడివి;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < సంకేతం_ranges.నిడివి; i++)
      {
	ఒకవేళ (unlikely (!((సంకేతం_ranges[i].సంకేతం_2 <= 0xFF) && (సంకేతం_ranges[i].glyph <= 0xFF))))
	  return_trace (అబద్ధం);
	fmt1->ranges[i].మొదటి = సంకేతం_ranges[i].సంకేతం_2;
	fmt1->ranges[i].n_ఎడమ = సంకేతం_ranges[i].glyph;
      }
    }
    ఆపు;

    }

    ఒకవేళ (supp_codes.నిడివి)
    {
      CFF1_Supp_Enc_దత్తాంశం *supp_దత్తాంశం = c->allocate_కొలత<CFF1_Supp_Enc_దత్తాంశం> (CFF1_Supp_Enc_దత్తాంశం::min_కొలత + SuppEncoding::static_కొలత * supp_codes.నిడివి);
      ఒకవేళ (unlikely (!supp_దత్తాంశం)) return_trace (అబద్ధం);
      supp_దత్తాంశం->nSups () = supp_codes.నిడివి;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < supp_codes.నిడివి; i++)
      {
	supp_దత్తాంశం->supps[i].సంకేతం_2 = supp_codes[i].సంకేతం_2;
	supp_దత్తాంశం->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (నిజం);
  }

  size_t get_కొలత () స్థిరరాశి
  {
    size_t కొలత = min_కొలత;
    ఎంపిక (పట్టిక_format ())
    {
    స్థితి 0: hb_barrier (); కొలత = hb_unsigned_కూడు_saturate (కొలత, u.format0.get_కొలత ()); ఆపు;
    స్థితి 1: hb_barrier (); కొలత = hb_unsigned_కూడు_saturate (కొలత, u.format1.get_కొలత ()); ఆపు;
    }
    ఒకవేళ (has_supplement ())
      కొలత = hb_unsigned_కూడు_saturate (కొలత, supp_Enc_దత్తాంశం ().get_కొలత ());
    తిరిగి కొలత;
  }

  hb_codepoint_t get_సంకేతం (hb_codepoint_t glyph) స్థిరరాశి
  {
    ఎంపిక (పట్టిక_format ())
    {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_సంకేతం (glyph);
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_సంకేతం (glyph);
    అప్రమేయ:తిరిగి 0;
    }
  }

  uint8_t పట్టిక_format () స్థిరరాశి { తిరిగి format & 0x7F; }
  తార్కిక  has_supplement () స్థిరరాశి { తిరిగి format & 0x80; }

  శూన్య get_supplement_codes (hb_codepoint_t sid, hb_సదిశ_t<hb_codepoint_t> &codes) స్థిరరాశి
  {
    codes.clear ();
    ఒకవేళ (has_supplement ())
      supp_Enc_దత్తాంశం().get_codes (sid, codes);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();

    ఎంపిక (పట్టిక_format ())
    {
    స్థితి 0: hb_barrier (); ఒకవేళ (unlikely (!u.format0.sanitize (c))) { return_trace (అబద్ధం); } ఆపు;
    స్థితి 1: hb_barrier (); ఒకవేళ (unlikely (!u.format1.sanitize (c))) { return_trace (అబద్ధం); } ఆపు;
    అప్రమేయ:return_trace (అబద్ధం);
    }
    return_trace (likely (!has_supplement () || supp_Enc_దత్తాంశం ().sanitize (c)));
  }

  రక్షిత:
  స్థిరరాశి CFF1_Supp_Enc_దత్తాంశం &supp_Enc_దత్తాంశం () స్థిరరాశి
  {
    ఎంపిక (పట్టిక_format ())
    {
    స్థితి 0: hb_barrier (); తిరిగి StructAfter<CFF1_Supp_Enc_దత్తాంశం> (u.format0.codes[u.format0.nCodes ()-1]);
    స్థితి 1: hb_barrier (); తిరిగి StructAfter<CFF1_Supp_Enc_దత్తాంశం> (u.format1.ranges[u.format1.nRanges ()-1]);
    అప్రమేయ:తిరిగి Null (CFF1_Supp_Enc_దత్తాంశం);
    }
  }

  బహిరంగ:
  HBUINT8	format;
  సంఘం {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_కొలత_MIN (1);
};

/* Charset */
నిర్మాణం Charset0
{
  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం అంకె_glyphs, గుర్తులేని *అంకె_charset_entries) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (అంకె_charset_entries) *అంకె_charset_entries = అంకె_glyphs;
    return_trace (sids.sanitize (c, అంకె_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    ఒకవేళ (unlikely (glyph >= అంకె_glyphs)) తిరిగి 0;
    ఒకవేళ (unlikely (glyph == 0))
      తిరిగి 0;
    లేకపోతే
      తిరిగి sids[glyph - 1];
  }

  శూన్య collect_glyph_గమ్యం_sid_map (glyph_గమ్యం_sid_map_t *mapping, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఒకవేళ (!mapping->resize_dirty  (అంకె_glyphs)) తిరిగి;
    కొరకు (hb_codepoint_t gid = 1; gid < అంకె_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఒకవేళ (sid == 0)
      తిరిగి 0;

    కొరకు (గుర్తులేని పూర్ణాంకం glyph = 1; glyph < అంకె_glyphs; glyph++)
    {
      ఒకవేళ (sids[glyph-1] == sid)
	తిరిగి glyph;
    }
    తిరిగి 0;
  }

  స్థిర size_t get_కొలత (గుర్తులేని పూర్ణాంకం అంకె_glyphs)
  {
    ప్రతిజ్ఞా (అంకె_glyphs > 0);
    తిరిగి Unsized_Array_యొక్క<HBUINT16>::get_కొలత (అంకె_glyphs - 1);
  }

  Unsized_Array_యొక్క<HBUINT16> sids;

  DEFINE_కొలత_ARRAY(0, sids);
};

మూస <రకం_పేరు TYPE>
నిర్మాణం Charset_Range {
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT16  మొదటి;
  TYPE      n_ఎడమ;

  DEFINE_కొలత_STATIC (HBUINT16::static_కొలత + TYPE::static_కొలత);
};

మూస <రకం_పేరు TYPE>
నిర్మాణం Charset1_2 {
  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం అంకె_glyphs, గుర్తులేని *అంకె_charset_entries) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    అంకె_glyphs--;
    గుర్తులేని i;
    కొరకు (i = 0; అంకె_glyphs > 0; i++)
    {
      ఒకవేళ (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (అంకె_glyphs >= ranges[i].n_ఎడమ + 1))))
	return_trace (అబద్ధం);
      అంకె_glyphs -= (ranges[i].n_ఎడమ + 1);
    }
    ఒకవేళ (అంకె_charset_entries)
      *అంకె_charset_entries = i;
    return_trace (నిజం);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, గుర్తులేని అంకె_glyphs,
			  సంకేతం_pair_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (unlikely (glyph >= అంకె_glyphs)) తిరిగి 0;
    గుర్తులేని i;
    hb_codepoint_t ప్రారంభం_glyph;
    ఒకవేళ (cache && likely (cache->glyph <= glyph))
    {
      i = cache->సంకేతం_2;
      ప్రారంభం_glyph = cache->glyph;
    }
    లేకపోతే
    {
      ఒకవేళ (unlikely (glyph == 0)) తిరిగి 0;
      i = 0;
      ప్రారంభం_glyph = 1;
    }
    glyph -= ప్రారంభం_glyph;
    కొరకు (;; i++)
    {
      గుర్తులేని లెక్క = ranges[i].n_ఎడమ;
      ఒకవేళ (glyph <= లెక్క)
      {
        ఒకవేళ (cache)
	  *cache = {i, ప్రారంభం_glyph};
	తిరిగి ranges[i].మొదటి + glyph;
      }
      లెక్క++;
      ప్రారంభం_glyph += లెక్క;
      glyph -= లెక్క;
    }

    తిరిగి 0;
  }

  శూన్య collect_glyph_గమ్యం_sid_map (glyph_గమ్యం_sid_map_t *mapping, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఒకవేళ (!mapping->resize_dirty  (అంకె_glyphs)) తిరిగి;

    hb_codepoint_t gid = 1;
    ఒకవేళ (gid >= అంకె_glyphs)
      తిరిగి;

    కొరకు (గుర్తులేని i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].మొదటి;
      గుర్తులేని లెక్క = ranges[i].n_ఎడమ + 1;
      గుర్తులేని చివరి = gid + లెక్క;
      కొరకు (గుర్తులేని j = 0; j < లెక్క; j++)
	mapping->arrayZ[gid++] = {sid++, చివరి - 1};

      ఒకవేళ (gid >= అంకె_glyphs)
        ఆపు;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఒకవేళ (sid == 0) తిరిగి 0;
    hb_codepoint_t  glyph = 1;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0;; i++)
    {
      ఒకవేళ (glyph >= అంకె_glyphs)
	తిరిగి 0;
      ఒకవేళ ((ranges[i].మొదటి <= sid) && (sid <= ranges[i].మొదటి + ranges[i].n_ఎడమ))
	తిరిగి glyph + (sid - ranges[i].మొదటి);
      glyph += (ranges[i].n_ఎడమ + 1);
    }

    తిరిగి 0;
  }

  size_t get_కొలత (గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    పూర్ణాంకం glyph = (పూర్ణాంకం) అంకె_glyphs;
    గుర్తులేని అంకె_ranges = 0;

    ప్రతిజ్ఞా (glyph > 0);
    glyph--;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_ఎడమ + 1);
      అంకె_ranges++;
    }

    తిరిగి get_కొలత_for_ranges (అంకె_ranges);
  }

  స్థిర size_t get_కొలత_for_ranges (గుర్తులేని పూర్ణాంకం అంకె_ranges)
  {
    తిరిగి Unsized_Array_యొక్క<Charset_Range<TYPE> >::get_కొలత (అంకె_ranges);
  }

  Unsized_Array_యొక్క<Charset_Range<TYPE>> ranges;

  DEFINE_కొలత_ARRAY (0, ranges);
};

రకనిర్వచనం Charset1_2<HBUINT8>     Charset1;
రకనిర్వచనం Charset1_2<HBUINT16>    Charset2;
రకనిర్వచనం Charset_Range<HBUINT8>  Charset1_Range;
రకనిర్వచనం Charset_Range<HBUINT16> Charset2_Range;

నిర్మాణం Charset
{
  /* serialize a fullset Charset */
  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి Charset &src, గుర్తులేని పూర్ణాంకం అంకె_glyphs)
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed ((స్థిరరాశి అక్షరం *) &src, src.get_కొలత (అంకె_glyphs)));
  }

  /* serialize a subset Charset */
  తార్కిక serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  గుర్తులేని పూర్ణాంకం అంకె_glyphs,
		  స్థిరరాశి hb_సదిశ_t<సంకేతం_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (ఇది);
    Charset *dest = c->extend_min (ఇది);
    ఒకవేళ (unlikely (!dest)) return_trace (అబద్ధం);
    dest->format = format;
    ఎంపిక (format)
    {
    స్థితి 0:
    {
      Charset0 *fmt0 = c->allocate_కొలత<Charset0> (Charset0::get_కొలత (అంకె_glyphs), అబద్ధం);
      ఒకవేళ (unlikely (!fmt0)) return_trace (అబద్ధం);
      గుర్తులేని పూర్ణాంకం glyph = 0;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < sid_ranges.నిడివి; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].సంకేతం_2;
	కొరకు (పూర్ణాంకం ఎడమ = (పూర్ణాంకం)sid_ranges.arrayZ[i].glyph; ఎడమ >= 0; ఎడమ--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    ఆపు;

    స్థితి 1:
    {
      Charset1 *fmt1 = c->allocate_కొలత<Charset1> (Charset1::get_కొలత_for_ranges (sid_ranges.నిడివి), అబద్ధం);
      ఒకవేళ (unlikely (!fmt1)) return_trace (అబద్ధం);
      hb_codepoint_t all_glyphs = 0;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < sid_ranges.నిడివి; i++)
      {
        స్వయంచాలక &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].మొదటి = _.సంకేతం_2;
	fmt1->ranges[i].n_ఎడమ = _.glyph;
      }
      ఒకవేళ (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (అబద్ధం);
    }
    ఆపు;

    స్థితి 2:
    {
      Charset2 *fmt2 = c->allocate_కొలత<Charset2> (Charset2::get_కొలత_for_ranges (sid_ranges.నిడివి), అబద్ధం);
      ఒకవేళ (unlikely (!fmt2)) return_trace (అబద్ధం);
      hb_codepoint_t all_glyphs = 0;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < sid_ranges.నిడివి; i++)
      {
        స్వయంచాలక &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].మొదటి = _.సంకేతం_2;
	fmt2->ranges[i].n_ఎడమ = _.glyph;
      }
      ఒకవేళ (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (అబద్ధం);
    }
    ఆపు;

    }
    return_trace (నిజం);
  }

  size_t get_కొలత (గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి hb_unsigned_కూడు_saturate (min_కొలత, u.format0.get_కొలత (అంకె_glyphs));
    స్థితి 1: hb_barrier (); తిరిగి hb_unsigned_కూడు_saturate (min_కొలత, u.format1.get_కొలత (అంకె_glyphs));
    స్థితి 2: hb_barrier (); తిరిగి hb_unsigned_కూడు_saturate (min_కొలత, u.format2.get_కొలత (అంకె_glyphs));
    అప్రమేయ:తిరిగి 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, గుర్తులేని పూర్ణాంకం అంకె_glyphs,
			  సంకేతం_pair_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_sid (glyph, అంకె_glyphs);
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_sid (glyph, అంకె_glyphs, cache);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_sid (glyph, అంకె_glyphs, cache);
    అప్రమేయ:తిరిగి 0;
    }
  }

  శూన్య collect_glyph_గమ్యం_sid_map (glyph_గమ్యం_sid_map_t *mapping, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); u.format0.collect_glyph_గమ్యం_sid_map (mapping, అంకె_glyphs); తిరిగి;
    స్థితి 1: hb_barrier (); u.format1.collect_glyph_గమ్యం_sid_map (mapping, అంకె_glyphs); తిరిగి;
    స్థితి 2: hb_barrier (); u.format2.collect_glyph_గమ్యం_sid_map (mapping, అంకె_glyphs); తిరిగి;
    అప్రమేయ:తిరిగి;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_glyph (sid, అంకె_glyphs);
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_glyph (sid, అంకె_glyphs);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_glyph (sid, అంకె_glyphs);
    అప్రమేయ:తిరిగి 0;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని *అంకె_charset_entries) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();

    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_అంకె_glyphs (), అంకె_charset_entries));
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_అంకె_glyphs (), అంకె_charset_entries));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_అంకె_glyphs (), అంకె_charset_entries));
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  HBUINT8       format;
  సంఘం {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_కొలత_MIN (1);
};

నిర్మాణం CFF1_String_సూచీ : CFF1_సూచీ
{
  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి CFF1_String_సూచీ &అక్షరశ్రేణులు,
		  స్థిరరాశి hb_సదిశ_t<గుర్తులేని> &sidmap)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely ((అక్షరశ్రేణులు.లెక్క == 0) || (sidmap.నిడివి == 0)))
    {
      ఒకవేళ (unlikely (!c->extend_min (ఇది->లెక్క)))
	return_trace (అబద్ధం);
      లెక్క = 0;
      return_trace (నిజం);
    }

    ఒకవేళ (unlikely (sidmap.లోపల_error ())) return_trace (అబద్ధం);

    // Save this in a vector since serialize() iterates it twice.
    hb_సదిశ_t<hb_ubytes_t> బైట్లు_Array (+ hb_iter (sidmap)
					 | hb_map (అక్షరశ్రేణులు));

    ఒకవేళ (unlikely (బైట్లు_Array.లోపల_error ())) return_trace (అబద్ధం);

    తార్కిక ఫలితం = CFF1_సూచీ::serialize (c, బైట్లు_Array);
    return_trace (ఫలితం);
  }
};

నిర్మాణం cff1_శిఖరం_dict_interp_env_t : అంకె_interp_env_t
{
  cff1_శిఖరం_dict_interp_env_t ()
    : అంకె_interp_env_t(), prev_స్థానభ్రంశం(0), చివరి_స్థానభ్రంశం(0) {}
  cff1_శిఖరం_dict_interp_env_t (స్థిరరాశి hb_ubytes_t &బైట్లు)
    : అంకె_interp_env_t(బైట్లు), prev_స్థానభ్రంశం(0), చివరి_స్థానభ్రంశం(0) {}

  గుర్తులేని పూర్ణాంకం prev_స్థానభ్రంశం;
  గుర్తులేని పూర్ణాంకం చివరి_స్థానభ్రంశం;
};

నిర్మాణం name_dict_విలువలు_t
{
  గణన name_dict_val_సూచీ_t
  {
      సంస్కరణ,
      notice,
      copyright,
      నిండిన_Name,
      familyName,
      బరువు,
      postscript,
      fontName,
      పునాది_Font_Name,
      registry,
      ordering,

      Val_లెక్క
  };

  శూన్య init ()
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < Val_లెక్క; i++)
      values[i] = CFF_UNDEF_SID;
  }

  గుర్తులేని పూర్ణాంకం& క్రియాకారకం[] (గుర్తులేని పూర్ణాంకం i)
  { ప్రతిజ్ఞా (i < Val_లెక్క); తిరిగి values[i]; }

  గుర్తులేని పూర్ణాంకం క్రియాకారకం[] (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { ప్రతిజ్ఞా (i < Val_లెక్క); తిరిగి values[i]; }

  స్థిర గణన name_dict_val_సూచీ_t name_op_గమ్యం_సూచీ (op_సంకేతం_t op)
  {
    ఎంపిక (op) {
      అప్రమేయ: // can't happen - just make some compiler happy
      స్థితి Op_సంకేతం_సంస్కరణ:
	తిరిగి సంస్కరణ;
      స్థితి Op_సంకేతం_Notice:
	తిరిగి notice;
      స్థితి Op_సంకేతం_Copyright:
	తిరిగి copyright;
      స్థితి Op_సంకేతం_నిండిన_Name:
	తిరిగి నిండిన_Name;
      స్థితి Op_సంకేతం_Family_Name:
	తిరిగి familyName;
      స్థితి Op_సంకేతం_బరువు:
	తిరిగి బరువు;
      స్థితి Op_సంకేతం_Post_Script:
	తిరిగి postscript;
      స్థితి Op_సంకేతం_Font_Name:
	తిరిగి fontName;
      స్థితి Op_సంకేతం_పునాది_Font_Name:
	తిరిగి పునాది_Font_Name;
    }
  }

  గుర్తులేని పూర్ణాంకం  values[Val_లెక్క];
};

నిర్మాణం cff1_శిఖరం_dict_val_t : op_str_t
{
  గుర్తులేని పూర్ణాంకం  చివరి_arg_స్థానభ్రంశం;
};

నిర్మాణం cff1_శిఖరం_dict_విలువలు_t : శిఖరం_dict_విలువలు_t<cff1_శిఖరం_dict_val_t>
{
  శూన్య init ()
  {
    శిఖరం_dict_విలువలు_t<cff1_శిఖరం_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_లెక్క = 8720;
    Encoding_స్థానభ్రంశం = 0;
    Charset_స్థానభ్రంశం = 0;
    FDSelect_స్థానభ్రంశం = 0;
    వ్యక్తిగత_Dict_Info.init ();
  }
  శూన్య fini () { శిఖరం_dict_విలువలు_t<cff1_శిఖరం_dict_val_t>::fini (); }

  తార్కిక is_CID () స్థిరరాశి
  { తిరిగి nameSIDs[name_dict_విలువలు_t::registry] != CFF_UNDEF_SID; }

  name_dict_విలువలు_t  nameSIDs;
  గుర్తులేని పూర్ణాంకం    ros_supplement_స్థానభ్రంశం;
  గుర్తులేని పూర్ణాంకం    ros_supplement;
  గుర్తులేని పూర్ణాంకం    cid_లెక్క;

  పూర్ణాంకం             Encoding_స్థానభ్రంశం;
  పూర్ణాంకం             Charset_స్థానభ్రంశం;
  పూర్ణాంకం             FDSelect_స్థానభ్రంశం;
  పట్టిక_info_t    వ్యక్తిగత_Dict_Info;
};

నిర్మాణం cff1_శిఖరం_dict_opset_t : శిఖరం_dict_opset_t<cff1_శిఖరం_dict_val_t>
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, cff1_శిఖరం_dict_interp_env_t& env, cff1_శిఖరం_dict_విలువలు_t& dictval)
  {
    cff1_శిఖరం_dict_val_t  val {};

    ఎంపిక (op) {
      స్థితి Op_సంకేతం_సంస్కరణ:
      స్థితి Op_సంకేతం_Notice:
      స్థితి Op_సంకేతం_Copyright:
      స్థితి Op_సంకేతం_నిండిన_Name:
      స్థితి Op_సంకేతం_Font_Name:
      స్థితి Op_సంకేతం_Family_Name:
      స్థితి Op_సంకేతం_బరువు:
      స్థితి Op_సంకేతం_Post_Script:
      స్థితి Op_సంకేతం_పునాది_Font_Name:
	dictval.nameSIDs[name_dict_విలువలు_t::name_op_గమ్యం_సూచీ (op)] = env.arg_దొంతర.తీయి_uint ();
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_is_Fixed_Pitch:
      స్థితి Op_సంకేతం_Italic_Angle:
      స్థితి Op_సంకేతం_Underline_Position:
      స్థితి Op_సంకేతం_Underline_Thickness:
      స్థితి Op_సంకేతం_Paint_Type:
      స్థితి Op_సంకేతం_Charstring_Type:
      స్థితి Op_సంకేతం_Unique_ID:
      స్థితి Op_సంకేతం_Stroke_వెడల్పు:
      స్థితి Op_సంకేతం_Synthetic_పునాది:
      స్థితి Op_సంకేతం_CIDFont_సంస్కరణ:
      స్థితి Op_సంకేతం_CIDFont_Revision:
      స్థితి Op_సంకేతం_CIDFont_Type:
      స్థితి Op_సంకేతం_UIDBase:
      స్థితి Op_సంకేతం_Font_BBox:
      స్థితి Op_సంకేతం_XUID:
      స్థితి Op_సంకేతం_పునాది_Font_Blend:
	env.clear_వాదనలు ();
	ఆపు;

      స్థితి Op_సంకేతం_CIDCount:
	dictval.cid_లెక్క = env.arg_దొంతర.తీయి_uint ();
	env.clear_వాదనలు ();
	ఆపు;

      స్థితి Op_సంకేతం_ROS:
	dictval.ros_supplement = env.arg_దొంతర.తీయి_uint ();
	dictval.nameSIDs[name_dict_విలువలు_t::ordering] = env.arg_దొంతర.తీయి_uint ();
	dictval.nameSIDs[name_dict_విలువలు_t::registry] = env.arg_దొంతర.తీయి_uint ();
	env.clear_వాదనలు ();
	ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;
	ప్రతిజ్ఞా (env.చివరి_స్థానభ్రంశం > dictval.op_ప్రారంభం);
	val.చివరి_arg_స్థానభ్రంశం = (env.చివరి_స్థానభ్రంశం - 1) - dictval.op_ప్రారంభం;  /* offset to the last argument */
	ఆపు;

      స్థితి Op_సంకేతం_Encoding:
	dictval.Encoding_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఒకవేళ (unlikely (dictval.Encoding_స్థానభ్రంశం == 0)) తిరిగి;
	ఆపు;

      స్థితి Op_సంకేతం_charset:
	dictval.Charset_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఒకవేళ (unlikely (dictval.Charset_స్థానభ్రంశం == 0)) తిరిగి;
	ఆపు;

      స్థితి Op_సంకేతం_FDSelect:
	dictval.FDSelect_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;

      స్థితి Op_సంకేతం_వ్యక్తిగత:
	dictval.వ్యక్తిగత_Dict_Info.స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	dictval.వ్యక్తిగత_Dict_Info.కొలత = env.arg_దొంతర.తీయి_uint ();
	env.clear_వాదనలు ();
	ఆపు;

      అప్రమేయ:
	env.చివరి_స్థానభ్రంశం = env.str_ref.get_స్థానభ్రంశం ();
	శిఖరం_dict_opset_t<cff1_శిఖరం_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
	ఆపు;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    dictval.కూడు_op (op, env.str_ref, val);
  }
};

నిర్మాణం cff1_font_dict_విలువలు_t : dict_విలువలు_t<op_str_t>
{
  శూన్య init ()
  {
    dict_విలువలు_t<op_str_t>::init ();
    వ్యక్తిగత_Dict_Info.init ();
    fontName = CFF_UNDEF_SID;
  }
  శూన్య fini () { dict_విలువలు_t<op_str_t>::fini (); }

  పట్టిక_info_t       వ్యక్తిగత_Dict_Info;
  గుర్తులేని పూర్ణాంకం    fontName;
};

నిర్మాణం cff1_font_dict_opset_t : dict_opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, అంకె_interp_env_t& env, cff1_font_dict_విలువలు_t& dictval)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_Font_Name:
	dictval.fontName = env.arg_దొంతర.తీయి_uint ();
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_Font_మాతృక:
      స్థితి Op_సంకేతం_Paint_Type:
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_వ్యక్తిగత:
	dictval.వ్యక్తిగత_Dict_Info.స్థానభ్రంశం = env.arg_దొంతర.తీయి_uint ();
	dictval.వ్యక్తిగత_Dict_Info.కొలత = env.arg_దొంతర.తీయి_uint ();
	env.clear_వాదనలు ();
	ఆపు;

      అప్రమేయ:
	dict_opset_t::process_op (op, env);
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
	ఆపు;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    dictval.కూడు_op (op, env.str_ref);
  }
};

మూస <రకం_పేరు VAL>
నిర్మాణం cff1_వ్యక్తిగత_dict_విలువలు_పునాది_t : dict_విలువలు_t<VAL>
{
  శూన్య init ()
  {
    dict_విలువలు_t<VAL>::init ();
    subrs_స్థానభ్రంశం = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  శూన్య fini () { dict_విలువలు_t<VAL>::fini (); }

  పూర్ణాంకం                 subrs_స్థానభ్రంశం;
  స్థిరరాశి CFF1Subrs    *localSubrs;
};

రకనిర్వచనం cff1_వ్యక్తిగత_dict_విలువలు_పునాది_t<op_str_t> cff1_వ్యక్తిగత_dict_విలువలు_subset_t;
రకనిర్వచనం cff1_వ్యక్తిగత_dict_విలువలు_పునాది_t<అంకె_dict_val_t> cff1_వ్యక్తిగత_dict_విలువలు_t;

నిర్మాణం cff1_వ్యక్తిగత_dict_opset_t : dict_opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, అంకె_interp_env_t& env, cff1_వ్యక్తిగత_dict_విలువలు_t& dictval)
  {
    అంకె_dict_val_t val;
    val.init ();

    ఎంపిక (op) {
      స్థితి Op_సంకేతం_Blue_విలువలు:
      స్థితి Op_సంకేతం_Other_Blues:
      స్థితి Op_సంకేతం_Family_Blues:
      స్థితి Op_సంకేతం_Family_Other_Blues:
      స్థితి Op_సంకేతం_Stem_Snap_H:
      స్థితి Op_సంకేతం_Stem_Snap_V:
      స్థితి Op_సంకేతం_Std_HW:
      స్థితి Op_సంకేతం_Std_VW:
      స్థితి Op_సంకేతం_Blue_Scale:
      స్థితి Op_సంకేతం_Blue_జరుపు:
      స్థితి Op_సంకేతం_Blue_Fuzz:
      స్థితి Op_సంకేతం_Force_Bold:
      స్థితి Op_సంకేతం_Language_సమూహం:
      స్థితి Op_సంకేతం_Expansion_గుణకం:
      స్థితి Op_సంకేతం_initial_Random_విత్తనం:
      స్థితి Op_సంకేతం_default_వెడల్పు_X:
      స్థితి Op_సంకేతం_nominal_వెడల్పు_X:
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_Subrs:
	dictval.subrs_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;

      అప్రమేయ:
	dict_opset_t::process_op (op, env);
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
	ఆపు;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    dictval.కూడు_op (op, env.str_ref, val);
  }
};

నిర్మాణం cff1_వ్యక్తిగత_dict_opset_subset_t : dict_opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, అంకె_interp_env_t& env, cff1_వ్యక్తిగత_dict_విలువలు_subset_t& dictval)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_Blue_విలువలు:
      స్థితి Op_సంకేతం_Other_Blues:
      స్థితి Op_సంకేతం_Family_Blues:
      స్థితి Op_సంకేతం_Family_Other_Blues:
      స్థితి Op_సంకేతం_Stem_Snap_H:
      స్థితి Op_సంకేతం_Stem_Snap_V:
      స్థితి Op_సంకేతం_Std_HW:
      స్థితి Op_సంకేతం_Std_VW:
      స్థితి Op_సంకేతం_Blue_Scale:
      స్థితి Op_సంకేతం_Blue_జరుపు:
      స్థితి Op_సంకేతం_Blue_Fuzz:
      స్థితి Op_సంకేతం_Force_Bold:
      స్థితి Op_సంకేతం_Language_సమూహం:
      స్థితి Op_సంకేతం_Expansion_గుణకం:
      స్థితి Op_సంకేతం_initial_Random_విత్తనం:
      స్థితి Op_సంకేతం_default_వెడల్పు_X:
      స్థితి Op_సంకేతం_nominal_వెడల్పు_X:
	env.clear_వాదనలు ();
	ఆపు;

      స్థితి Op_సంకేతం_Subrs:
	dictval.subrs_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;

      అప్రమేయ:
	dict_opset_t::process_op (op, env);
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
	ఆపు;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    dictval.కూడు_op (op, env.str_ref);
  }
};

రకనిర్వచనం dict_interpreter_t<cff1_శిఖరం_dict_opset_t, cff1_శిఖరం_dict_విలువలు_t, cff1_శిఖరం_dict_interp_env_t> cff1_శిఖరం_dict_interpreter_t;
రకనిర్వచనం dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_విలువలు_t> cff1_font_dict_interpreter_t;

రకనిర్వచనం CFF1_సూచీ CFF1_Name_సూచీ;
రకనిర్వచనం CFF1_సూచీ CFF1_శిఖరం_Dict_సూచీ;

నిర్మాణం cff1_font_dict_విలువలు_mod_t
{
  cff1_font_dict_విలువలు_mod_t() { init (); }

  శూన్య init () { init ( &Null (cff1_font_dict_విలువలు_t), CFF_UNDEF_SID ); }

  శూన్య init (స్థిరరాశి cff1_font_dict_విలువలు_t *పునాది_2,
	     గుర్తులేని పూర్ణాంకం fontName_)
  {
    పునాది = పునాది_2;
    fontName = fontName_;
    వ్యక్తిగత_Dict_Info.init ();
  }

  గుర్తులేని get_లెక్క () స్థిరరాశి { తిరిగి పునాది->get_లెక్క (); }

  స్థిరరాశి op_str_t &క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి { తిరిగి (*పునాది)[i]; }

  స్థిరరాశి cff1_font_dict_విలువలు_t    *పునాది;
  పట్టిక_info_t		   వ్యక్తిగత_Dict_Info;
  గుర్తులేని పూర్ణాంకం		fontName;
};

నిర్మాణం CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  మూస <రకం_పేరు ITER, రకం_పేరు OP_SERIALIZER>
  తార్కిక serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { తిరిగి FDArray<HBUINT16>::serialize<cff1_font_dict_విలువలు_mod_t, cff1_font_dict_విలువలు_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

నామాకాశ OT {

ఉపయోగించి నామాకాశ CFF;

నిర్మాణం cff1
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_CFF1;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1));
  }

  మూస <రకం_పేరు PRIVOPSET, రకం_పేరు PRIVDICTVAL>
  నిర్మాణం accelerator_templ_t
  {
    స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = cff1::పట్టిక_ట్యాగ్;

    accelerator_templ_t (hb_face_t *face)
    {
      ఒకవేళ (!face) తిరిగి;

      శిఖరం_Dict_2.init ();
      fontDicts.init ();
      వ్యక్తిగత_Dicts.init ();

      ఇది->blob = sc.reference_పట్టిక<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (ఇది->blob);
      sc.ప్రారంభం_processing ();

      స్థిరరాశి OT::cff1 *cff = ఇది->blob->మూస as<OT::cff1> ();

      ఒకవేళ (cff == &Null (OT::cff1))
        వెళ్ళు fail;

      name_సూచీ = &cff->name_సూచీ (cff);
      ఒకవేళ ((name_సూచీ == &Null (CFF1_Name_సూచీ)) || !name_సూచీ->sanitize (&sc))
        వెళ్ళు fail;
      hb_barrier ();

      శిఖరం_Dict_సూచీ = &Struct_At_స్థానభ్రంశం_Or_Null<CFF1_శిఖరం_Dict_సూచీ> (name_సూచీ, name_సూచీ->get_కొలత (), sc);
      ఒకవేళ (శిఖరం_Dict_సూచీ == &Null (CFF1_శిఖరం_Dict_సూచీ) || (శిఖరం_Dict_సూచీ->లెక్క == 0))
        వెళ్ళు fail;
      hb_barrier ();

      { /* parse top dict */
	స్థిరరాశి hb_ubytes_t శిఖరం_Dict_Str = (*శిఖరం_Dict_సూచీ)[0];
	ఒకవేళ (unlikely (!శిఖరం_Dict_Str.sanitize (&sc)))   వెళ్ళు fail;
	hb_barrier ();
	cff1_శిఖరం_dict_interp_env_t env (శిఖరం_Dict_Str);
	cff1_శిఖరం_dict_interpreter_t శిఖరం_interp (env);
	ఒకవేళ (unlikely (!శిఖరం_interp.interpret (శిఖరం_Dict_2)))   వెళ్ళు fail;
      }

      ఒకవేళ (is_predef_charset ())
	charset = &Null (Charset);
      లేకపోతే
      {
	charset = &Struct_At_స్థానభ్రంశం_Or_Null<Charset> (cff, శిఖరం_Dict_2.Charset_స్థానభ్రంశం, sc, &అంకె_charset_entries);
	ఒకవేళ (unlikely (charset == &Null (Charset)))   వెళ్ళు fail;
      }

      fd_లెక్క = 1;
      ఒకవేళ (is_CID ())
      {
	fdArray = &Struct_At_స్థానభ్రంశం_Or_Null<CFF1FDArray> (cff, శిఖరం_Dict_2.FDArray_స్థానభ్రంశం, sc);
	fdSelect = &Struct_At_స్థానభ్రంశం_Or_Null<CFF1FDSelect> (cff, శిఖరం_Dict_2.FDSelect_స్థానభ్రంశం, sc, fdArray->లెక్క);
	ఒకవేళ (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  వెళ్ళు fail;

	fd_లెక్క = fdArray->లెక్క;
      }
      లేకపోతే
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      ఒకవేళ (is_CID ())
      {
	ఒకవేళ (unlikely (charset == &Null (Charset)))   వెళ్ళు fail;
      }
      లేకపోతే
      {
	ఒకవేళ (!is_predef_encoding ())
	{
	  encoding = &Struct_At_స్థానభ్రంశం_Or_Null<Encoding> (cff, శిఖరం_Dict_2.Encoding_స్థానభ్రంశం, sc);
	  ఒకవేళ (unlikely (encoding == &Null (Encoding)))   వెళ్ళు fail;
	}
      }

      string_సూచీ = &Struct_At_స్థానభ్రంశం_Or_Null<CFF1_String_సూచీ> (శిఖరం_Dict_సూచీ, శిఖరం_Dict_సూచీ->get_కొలత (), sc);
      ఒకవేళ (string_సూచీ == &Null (CFF1_String_సూచీ))
        వెళ్ళు fail;

      globalSubrs = &Struct_At_స్థానభ్రంశం_Or_Null<CFF1Subrs> (string_సూచీ, string_సూచీ->get_కొలత (), sc);
      char_అక్షరశ్రేణులు = &Struct_At_స్థానభ్రంశం_Or_Null<CFF1_Char_అక్షరశ్రేణులు> (cff, శిఖరం_Dict_2.char_అక్షరశ్రేణులు_స్థానభ్రంశం, sc);
      ఒకవేళ (char_అక్షరశ్రేణులు == &Null (CFF1_Char_అక్షరశ్రేణులు))
        వెళ్ళు fail;

      అంకె_glyphs = char_అక్షరశ్రేణులు->లెక్క;
      ఒకవేళ (అంకె_glyphs != sc.get_అంకె_glyphs ())
        వెళ్ళు fail;

      ఒకవేళ (unlikely (!వ్యక్తిగత_Dicts.resize (fd_లెక్క)))
        వెళ్ళు fail;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < fd_లెక్క; i++)
	వ్యక్తిగత_Dicts[i].init ();

      // parse CID font dicts and gather private dicts
      ఒకవేళ (is_CID ())
      {
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < fd_లెక్క; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  ఒకవేళ (unlikely (!fontDictStr.sanitize (&sc)))   వెళ్ళు fail;
	  hb_barrier ();
	  cff1_font_dict_విలువలు_t *font;
	  cff1_శిఖరం_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  ఒకవేళ (unlikely (fontDicts.లోపల_error ()))   వెళ్ళు fail;

	  font->init ();
	  ఒకవేళ (unlikely (!font_interp.interpret (*font)))   వెళ్ళు fail;
	  PRIVDICTVAL *priv = &వ్యక్తిగత_Dicts[i];
	  స్థిరరాశి hb_ubytes_t privDictStr = Struct_At_స్థానభ్రంశం_Or_Null<UnsizedByteStr> (cff, font->వ్యక్తిగత_Dict_Info.స్థానభ్రంశం, sc, font->వ్యక్తిగత_Dict_Info.కొలత).as_ubytes (font->వ్యక్తిగత_Dict_Info.కొలత);
	  ఒకవేళ (unlikely (font->వ్యక్తిగత_Dict_Info.కొలత &&
			privDictStr == (స్థిరరాశి గుర్తులేని అక్షరం *) &Null (UnsizedByteStr))) వెళ్ళు fail;
	  అంకె_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  ఒకవేళ (unlikely (!priv_interp.interpret (*priv)))   వెళ్ళు fail;

	  priv->localSubrs = &Struct_At_స్థానభ్రంశం_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_స్థానభ్రంశం, sc);
	}
      }
      లేకపోతే  /* non-CID */
      {
	cff1_శిఖరం_dict_విలువలు_t *font = &శిఖరం_Dict_2;
	PRIVDICTVAL *priv = &వ్యక్తిగత_Dicts[0];

	స్థిరరాశి hb_ubytes_t privDictStr = Struct_At_స్థానభ్రంశం_Or_Null<UnsizedByteStr> (cff, font->వ్యక్తిగత_Dict_Info.స్థానభ్రంశం, sc, font->వ్యక్తిగత_Dict_Info.కొలత).as_ubytes (font->వ్యక్తిగత_Dict_Info.కొలత);
	ఒకవేళ (font->వ్యక్తిగత_Dict_Info.కొలత &&
	    unlikely (privDictStr == (స్థిరరాశి గుర్తులేని అక్షరం *) &Null (UnsizedByteStr))) వెళ్ళు fail;
	అంకె_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	ఒకవేళ (unlikely (!priv_interp.interpret (*priv)))   వెళ్ళు fail;

	priv->localSubrs = &Struct_At_స్థానభ్రంశం_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_స్థానభ్రంశం, sc);
	hb_barrier ();
      }

      తిరిగి;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    శూన్య _fini ()
    {
      sc.చివర_processing ();
      శిఖరం_Dict_2.fini ();
      fontDicts.fini ();
      వ్యక్తిగత_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = శూన్యసూచిక;
    }

    hb_blob_t *get_blob () స్థిరరాశి { తిరిగి blob; }

    తార్కిక is_valid () స్థిరరాశి { తిరిగి blob; }
    తార్కిక   is_CID () స్థిరరాశి { తిరిగి శిఖరం_Dict_2.is_CID (); }

    తార్కిక is_predef_charset () స్థిరరాశి { తిరిగి శిఖరం_Dict_2.Charset_స్థానభ్రంశం <= ExpertSubsetCharset; }

    గుర్తులేని పూర్ణాంకం std_సంకేతం_గమ్యం_glyph (hb_codepoint_t సంకేతం_2) స్థిరరాశి
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (సంకేతం_2);
      ఒకవేళ (unlikely (sid == CFF_UNDEF_SID))
	తిరిగి 0;

      ఒకవేళ (charset != &Null (Charset))
	తిరిగి charset->get_glyph (sid, అంకె_glyphs);
      లేకపోతే ఒకవేళ ((శిఖరం_Dict_2.Charset_స్థానభ్రంశం == ISOAdobeCharset)
	      && (సంకేతం_2 <= 228 /*zcaron*/)) తిరిగి sid;
      తిరిగి 0;
    }

    తార్కిక is_predef_encoding () స్థిరరాశి { తిరిగి శిఖరం_Dict_2.Encoding_స్థానభ్రంశం <= ExpertEncoding; }

    hb_codepoint_t glyph_గమ్యం_సంకేతం (hb_codepoint_t glyph,
				  సంకేతం_pair_t *glyph_గమ్యం_sid_cache = శూన్యసూచిక) స్థిరరాశి
    {
      ఒకవేళ (encoding != &Null (Encoding))
	తిరిగి encoding->get_సంకేతం (glyph);
      లేకపోతే
      {
	hb_codepoint_t sid = glyph_గమ్యం_sid (glyph, glyph_గమ్యం_sid_cache);
	ఒకవేళ (sid == 0) తిరిగి 0;
	hb_codepoint_t సంకేతం_2 = 0;
	ఎంపిక (శిఖరం_Dict_2.Encoding_స్థానభ్రంశం)
	{
	స్థితి StandardEncoding:
	  సంకేతం_2 = lookup_standard_encoding_for_సంకేతం (sid);
	  ఆపు;
	స్థితి ExpertEncoding:
	  సంకేతం_2 = lookup_expert_encoding_for_సంకేతం (sid);
	  ఆపు;
	అప్రమేయ:
	  ఆపు;
	}
	తిరిగి సంకేతం_2;
      }
    }

    glyph_గమ్యం_sid_map_t *create_glyph_గమ్యం_sid_map () స్థిరరాశి
    {
      ఒకవేళ (charset != &Null (Charset))
      {
	స్వయంచాలక *mapping = (glyph_గమ్యం_sid_map_t *) hb_malloc (పరిమాణం (glyph_గమ్యం_sid_map_t));
	ఒకవేళ (unlikely (!mapping)) తిరిగి శూన్యసూచిక;
	mapping = కొత్త (mapping) glyph_గమ్యం_sid_map_t ();
	mapping->push (సంకేతం_pair_t {0, 1});
	charset->collect_glyph_గమ్యం_sid_map (mapping, అంకె_glyphs);
	తిరిగి mapping;
      }
      లేకపోతే
	తిరిగి శూన్యసూచిక;
    }

    hb_codepoint_t glyph_గమ్యం_sid (hb_codepoint_t glyph,
				 సంకేతం_pair_t *cache = శూన్యసూచిక) స్థిరరాశి
    {
      ఒకవేళ (charset != &Null (Charset))
	తిరిగి charset->get_sid (glyph, అంకె_glyphs, cache);
      లేకపోతే
      {
	hb_codepoint_t sid = 0;
	ఎంపిక (శిఖరం_Dict_2.Charset_స్థానభ్రంశం)
	{
	  స్థితి ISOAdobeCharset:
	    ఒకవేళ (glyph <= 228 /*zcaron*/) sid = glyph;
	    ఆపు;
	  స్థితి ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    ఆపు;
	  స్థితి ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    ఆపు;
	  అప్రమేయ:
	    ఆపు;
	}
	తిరిగి sid;
      }
    }

    hb_codepoint_t sid_గమ్యం_glyph (hb_codepoint_t sid) స్థిరరాశి
    {
      ఒకవేళ (charset != &Null (Charset))
	తిరిగి charset->get_glyph (sid, అంకె_glyphs);
      లేకపోతే
      {
	hb_codepoint_t glyph = 0;
	ఎంపిక (శిఖరం_Dict_2.Charset_స్థానభ్రంశం)
	{
	  స్థితి ISOAdobeCharset:
	    ఒకవేళ (sid <= 228 /*zcaron*/) glyph = sid;
	    ఆపు;
	  స్థితి ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    ఆపు;
	  స్థితి ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    ఆపు;
	  అప్రమేయ:
	    ఆపు;
	}
	తిరిగి glyph;
      }
    }

    రక్షిత:
    hb_sanitize_context_t   sc;

    బహిరంగ:
    hb_blob_t               *blob = శూన్యసూచిక;
    స్థిరరాశి Encoding	    *encoding = శూన్యసూచిక;
    స్థిరరాశి Charset	    *charset = శూన్యసూచిక;
    స్థిరరాశి CFF1_Name_సూచీ     *name_సూచీ = శూన్యసూచిక;
    స్థిరరాశి CFF1_శిఖరం_Dict_సూచీ  *శిఖరం_Dict_సూచీ = శూన్యసూచిక;
    స్థిరరాశి CFF1_String_సూచీ   *string_సూచీ = శూన్యసూచిక;
    స్థిరరాశి CFF1Subrs	    *globalSubrs = శూన్యసూచిక;
    స్థిరరాశి CFF1_Char_అక్షరశ్రేణులు   *char_అక్షరశ్రేణులు = శూన్యసూచిక;
    స్థిరరాశి CFF1FDArray       *fdArray = శూన్యసూచిక;
    స్థిరరాశి CFF1FDSelect      *fdSelect = శూన్యసూచిక;
    గుర్తులేని పూర్ణాంకం	     fd_లెక్క = 0;

    cff1_శిఖరం_dict_విలువలు_t   శిఖరం_Dict_2;
    hb_సదిశ_t<cff1_font_dict_విలువలు_t>
			     fontDicts;
    hb_సదిశ_t<PRIVDICTVAL> వ్యక్తిగత_Dicts;

    గుర్తులేని పూర్ణాంకం	     అంకె_glyphs = 0;
    గుర్తులేని పూర్ణాంకం	     అంకె_charset_entries = 0;
  };

  నిర్మాణం accelerator_t : accelerator_templ_t<cff1_వ్యక్తిగత_dict_opset_t, cff1_వ్యక్తిగత_dict_విలువలు_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_పేర్లు.సమితి_relaxed (శూన్యసూచిక);

      ఒకవేళ (!is_valid ()) తిరిగి;
      ఒకవేళ (is_CID ()) తిరిగి;
    }
    ~accelerator_t ()
    {
      hb_sorted_సదిశ_t<gname_t> *పేర్లు = glyph_పేర్లు.get_relaxed ();
      ఒకవేళ (పేర్లు)
      {
	పేర్లు->fini ();
	hb_free (పేర్లు);
      }
    }

    తార్కిక get_glyph_name (hb_codepoint_t glyph,
			 అక్షరం *buf, గుర్తులేని పూర్ణాంకం buf_len) స్థిరరాశి
    {
      ఒకవేళ (unlikely (glyph >= అంకె_glyphs)) తిరిగి అబద్ధం;
      ఒకవేళ (unlikely (!is_valid ())) తిరిగి అబద్ధం;
      ఒకవేళ (is_CID()) తిరిగి అబద్ధం;
      ఒకవేళ (unlikely (!buf_len)) తిరిగి నిజం;
      hb_codepoint_t sid = glyph_గమ్యం_sid (glyph);
      స్థిరరాశి అక్షరం *str;
      size_t str_len;
      ఒకవేళ (sid < cff1_std_అక్షరశ్రేణులు_నిడివి)
      {
	hb_బైట్లు_t byte_str = cff1_std_అక్షరశ్రేణులు (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.నిడివి;
      }
      లేకపోతే
      {
	hb_ubytes_t ubyte_str = (*string_సూచీ)[sid - cff1_std_అక్షరశ్రేణులు_నిడివి];
	str = (స్థిరరాశి అక్షరం *)ubyte_str.arrayZ;
	str_len = ubyte_str.నిడివి;
      }
      ఒకవేళ (!str_len) తిరిగి అబద్ధం;
      గుర్తులేని పూర్ణాంకం len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (స్థిరరాశి అక్షరం*)str, len);
      buf[len] = '\0';
      తిరిగి నిజం;
    }

    తార్కిక get_glyph_from_name (స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
			      hb_codepoint_t *glyph) స్థిరరాశి
    {
      ఒకవేళ (unlikely (!is_valid ())) తిరిగి అబద్ధం;
      ఒకవేళ (is_CID()) తిరిగి అబద్ధం;
      ఒకవేళ (len < 0) len = అక్షరశ్రేణి_పొడవు (name);
      ఒకవేళ (unlikely (!len)) తిరిగి అబద్ధం;

    retry:
      hb_sorted_సదిశ_t<gname_t> *పేర్లు = glyph_పేర్లు.get_acquire ();
      ఒకవేళ (unlikely (!పేర్లు))
      {
	పేర్లు = (hb_sorted_సదిశ_t<gname_t> *) hb_calloc (1, పరిమాణం (hb_sorted_సదిశ_t<gname_t>));
	ఒకవేళ (likely (పేర్లు))
	{
	  పేర్లు->init ();
	  /* TODO */

	  /* fill glyph names */
	  సంకేతం_pair_t glyph_గమ్యం_sid_cache {0, HB_CODEPOINT_INVALID};
	  కొరకు (hb_codepoint_t gid = 0; gid < అంకె_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_గమ్యం_sid (gid, &glyph_గమ్యం_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    ఒకవేళ (sid < cff1_std_అక్షరశ్రేణులు_నిడివి)
	      gname.name = cff1_std_అక్షరశ్రేణులు (sid);
	    లేకపోతే
	    {
	      hb_ubytes_t	ustr = (*string_సూచీ)[sid - cff1_std_అక్షరశ్రేణులు_నిడివి];
	      gname.name = hb_బైట్లు_t ((స్థిరరాశి అక్షరం*) ustr.arrayZ, ustr.నిడివి);
	    }
	    ఒకవేళ (unlikely (!gname.name.arrayZ))
	      gname.name = hb_బైట్లు_t ("", 0); /* To avoid nullptr. */
	    పేర్లు->push (gname);
	  }
	  పేర్లు->qsort ();
	}
	ఒకవేళ (unlikely (!glyph_పేర్లు.cmpexch (శూన్యసూచిక, పేర్లు)))
	{
	  ఒకవేళ (పేర్లు)
	  {
	    పేర్లు->fini ();
	    hb_free (పేర్లు);
	  }
	  వెళ్ళు retry;
	}
      }

      gname_t key = { hb_బైట్లు_t (name, len), 0 };
      స్థిరరాశి gname_t *gname = పేర్లు ? పేర్లు->bsearch (key) : శూన్యసూచిక;
      ఒకవేళ (!gname) తిరిగి అబద్ధం;
      hb_codepoint_t gid = sid_గమ్యం_glyph (gname->sid);
      ఒకవేళ (!gid && gname->sid) తిరిగి అబద్ధం;
      *glyph = gid;
      తిరిగి నిజం;
    }

    HB_అంతర్గత తార్కిక get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = శూన్యసూచిక) స్థిరరాశి;
    HB_అంతర్గత తార్కిక get_మార్గం (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = శూన్యసూచిక) స్థిరరాశి;

    అంతరంగ:
    నిర్మాణం gname_t
    {
      hb_బైట్లు_t	name;
      uint16_t		sid;

      స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *a_, స్థిరరాశి శూన్య *b_)
      {
	స్థిరరాశి gname_t *a = (స్థిరరాశి gname_t *)a_;
	స్థిరరాశి gname_t *b = (స్థిరరాశి gname_t *)b_;
	గుర్తులేని minlen = hb_min (a->name.నిడివి, b->name.నిడివి);
	పూర్ణాంకం ret = అక్షరశ్రేణి_పోలిక (a->name.arrayZ, b->name.arrayZ, minlen);
	ఒకవేళ (ret) తిరిగి ret;
	తిరిగి a->name.నిడివి - b->name.నిడివి;
      }

      పూర్ణాంకం cmp (స్థిరరాశి gname_t &a) స్థిరరాశి { తిరిగి cmp (&a, ఇది); }
    };

    మార్చదగిన hb_atomic_t<hb_sorted_సదిశ_t<gname_t> *> glyph_పేర్లు;

    రకనిర్వచనం accelerator_templ_t<cff1_వ్యక్తిగత_dict_opset_t, cff1_వ్యక్తిగత_dict_విలువలు_t> SUPER;
  };

  నిర్మాణం accelerator_subset_t : accelerator_templ_t<cff1_వ్యక్తిగత_dict_opset_subset_t, cff1_వ్యక్తిగత_dict_విలువలు_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ఒకవేళ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_అంతర్గత తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి;
    HB_అంతర్గత తార్కిక serialize (hb_serialize_context_t *c,
				నిర్మాణం cff1_subset_plan &plan) స్థిరరాశి;
    HB_అంతర్గత తార్కిక get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *పునాది, hb_codepoint_t *accent) స్థిరరాశి;

    మార్చదగిన CFF::cff_subset_accelerator_t* cff_accelerator = శూన్యసూచిక;

    రకనిర్వచనం accelerator_templ_t<cff1_వ్యక్తిగత_dict_opset_subset_t, cff1_వ్యక్తిగత_dict_విలువలు_subset_t> SUPER;
  };

  రక్షిత:
  HB_అంతర్గత స్థిర hb_codepoint_t lookup_standard_encoding_for_సంకేతం (hb_codepoint_t sid);
  HB_అంతర్గత స్థిర hb_codepoint_t lookup_expert_encoding_for_సంకేతం (hb_codepoint_t sid);
  HB_అంతర్గత స్థిర hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_అంతర్గత స్థిర hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_అంతర్గత స్థిర hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_అంతర్గత స్థిర hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_అంతర్గత స్థిర hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t సంకేతం_2);

  బహిరంగ:
  Fixed_సంస్కరణ<HBUINT8> సంస్కరణ;	  /* Version of CFF table. set to 0x0100u */
  NNOffset_గమ్యం<CFF1_Name_సూచీ, HBUINT8> name_సూచీ; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_కొలత;	  /* offset size (unused?) */

  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

నిర్మాణం cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  శూన్య depend (hb_depend_దత్తాంశం_builder_t *builder) స్థిరరాశి
  {
    ఒకవేళ (!is_valid ()) తిరిగి;
    కొరకు (hb_codepoint_t gid = 0; gid < అంకె_glyphs; gid++)
    {
      hb_codepoint_t పునాది_gid, accent_gid;
      ఒకవేళ (get_seac_components (gid, &పునాది_gid, &accent_gid))
      {
        builder->కూడు_depend (gid, HB_ట్యాగ్('C','F','F',' '), పునాది_gid);
        builder->కూడు_depend (gid, HB_ట్యాగ్('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#అయితే_ముగింపు /* HB_OT_CFF1_TABLE_HH */
