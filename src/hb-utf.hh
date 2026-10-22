/*
 * Copyright © 2011,2012,2014  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_UTF_HH
#నిర్వచించు HB_UTF_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-open-type.hh"


నిర్మాణం hb_utf8_t
{
  రకనిర్వచనం uint8_t codepoint_t;
  స్థిర constexpr గుర్తులేని max_len = 4;

  స్థిర అంతఃవరుస స్థిరరాశి codepoint_t *
  next (స్థిరరాశి codepoint_t *పాఠ్యం,
	స్థిరరాశి codepoint_t *చివర,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *పాఠ్యం++;

    ఒకవేళ (c > 0x7Fu)
    {
      ఒకవేళ (hb_లోపల_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	గుర్తులేని పూర్ణాంకం t1;
	ఒకవేళ (likely (పాఠ్యం < చివర &&
		    (t1 = పాఠ్యం[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  పాఠ్యం++;
	}
	లేకపోతే
	  వెళ్ళు error;
      }
      లేకపోతే ఒకవేళ (hb_లోపల_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	గుర్తులేని పూర్ణాంకం t1, t2;
	ఒకవేళ (likely (1 < చివర - పాఠ్యం &&
		    (t1 = పాఠ్యం[0] - 0x80u) <= 0x3Fu &&
		    (t2 = పాఠ్యం[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  ఒకవేళ (unlikely (c < 0x0800u || hb_లోపల_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    వెళ్ళు error;
	  పాఠ్యం += 2;
	}
	లేకపోతే
	  వెళ్ళు error;
      }
      లేకపోతే ఒకవేళ (hb_లోపల_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	గుర్తులేని పూర్ణాంకం t1, t2, t3;
	ఒకవేళ (likely (2 < చివర - పాఠ్యం &&
		    (t1 = పాఠ్యం[0] - 0x80u) <= 0x3Fu &&
		    (t2 = పాఠ్యం[1] - 0x80u) <= 0x3Fu &&
		    (t3 = పాఠ్యం[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  ఒకవేళ (unlikely (!hb_లోపల_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    వెళ్ళు error;
	  పాఠ్యం += 3;
	}
	లేకపోతే
	  వెళ్ళు error;
      }
      లేకపోతే
	వెళ్ళు error;
    }

    *unicode = c;
    తిరిగి పాఠ్యం;

  error:
    *unicode = replacement;
    తిరిగి పాఠ్యం;
  }

  స్థిర అంతఃవరుస స్థిరరాశి codepoint_t *
  prev (స్థిరరాశి codepoint_t *పాఠ్యం,
	స్థిరరాశి codepoint_t *ప్రారంభం,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    స్థిరరాశి codepoint_t *చివర = పాఠ్యం--;
    అయితే (ప్రారంభం < పాఠ్యం && (*పాఠ్యం & 0xc0) == 0x80 && చివర - పాఠ్యం < 4)
      పాఠ్యం--;

    ఒకవేళ (likely (next (పాఠ్యం, చివర, unicode, replacement) == చివర))
      తిరిగి పాఠ్యం;

    *unicode = replacement;
    తిరిగి చివర - 1;
  }

  స్థిర గుర్తులేని పూర్ణాంకం
  అక్షరశ్రేణి_పొడవు (స్థిరరాశి codepoint_t *పాఠ్యం)
  { తిరిగి ::అక్షరశ్రేణి_పొడవు ((స్థిరరాశి అక్షరం *) పాఠ్యం); }

  స్థిర గుర్తులేని పూర్ణాంకం
  encode_len (hb_codepoint_t unicode)
  {
    ఒకవేళ (unicode <   0x0080u) తిరిగి 1;
    ఒకవేళ (unicode <   0x0800u) తిరిగి 2;
    ఒకవేళ (unicode <  0x10000u) తిరిగి 3;
    ఒకవేళ (unicode < 0x110000u) తిరిగి 4;
    తిరిగి 3;
  }

  స్థిర codepoint_t *
  encode (codepoint_t *పాఠ్యం,
	  స్థిరరాశి codepoint_t *చివర,
	  hb_codepoint_t unicode)
  {
    ఒకవేళ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    ఒకవేళ (unicode < 0x0080u)
     *పాఠ్యం++ = unicode;
    లేకపోతే ఒకవేళ (unicode < 0x0800u)
    {
      ఒకవేళ (చివర - పాఠ్యం >= 2)
      {
	*పాఠ్యం++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*పాఠ్యం++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    లేకపోతే ఒకవేళ (unicode < 0x10000u)
    {
      ఒకవేళ (చివర - పాఠ్యం >= 3)
      {
	*పాఠ్యం++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*పాఠ్యం++ =  0x80u + (0x3Fu & (unicode >>  6));
	*పాఠ్యం++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    లేకపోతే
    {
      ఒకవేళ (చివర - పాఠ్యం >= 4)
      {
	*పాఠ్యం++ =  0xF0u + (0x07u & (unicode >> 18));
	*పాఠ్యం++ =  0x80u + (0x3Fu & (unicode >> 12));
	*పాఠ్యం++ =  0x80u + (0x3Fu & (unicode >>  6));
	*పాఠ్యం++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    తిరిగి పాఠ్యం;
  }
};


మూస <రకం_పేరు TCodepoint>
నిర్మాణం hb_utf16_xe_t
{
  static_assert (పరిమాణం (TCodepoint) == 2, "");
  రకనిర్వచనం TCodepoint codepoint_t;
  స్థిర constexpr గుర్తులేని max_len = 2;

  స్థిర అంతఃవరుస స్థిరరాశి codepoint_t *
  next (స్థిరరాశి codepoint_t *పాఠ్యం,
	స్థిరరాశి codepoint_t *చివర,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *పాఠ్యం++;

    ఒకవేళ (likely (!hb_లోపల_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      తిరిగి పాఠ్యం;
    }

    ఒకవేళ (likely (c <= 0xDBFFu && పాఠ్యం < చివర))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *పాఠ్యం;
      ఒకవేళ (likely (hb_లోపల_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 పాఠ్యం++;
	 తిరిగి పాఠ్యం;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    తిరిగి పాఠ్యం;
  }

  స్థిర అంతఃవరుస స్థిరరాశి codepoint_t *
  prev (స్థిరరాశి codepoint_t *పాఠ్యం,
	స్థిరరాశి codepoint_t *ప్రారంభం,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--పాఠ్యం;

    ఒకవేళ (likely (!hb_లోపల_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      తిరిగి పాఠ్యం;
    }

    ఒకవేళ (likely (c >= 0xDC00u && ప్రారంభం < పాఠ్యం))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = పాఠ్యం[-1];
      ఒకవేళ (likely (hb_లోపల_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	పాఠ్యం--;
	తిరిగి పాఠ్యం;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    తిరిగి పాఠ్యం;
  }


  స్థిర గుర్తులేని పూర్ణాంకం
  అక్షరశ్రేణి_పొడవు (స్థిరరాశి codepoint_t *పాఠ్యం)
  {
    గుర్తులేని పూర్ణాంకం l = 0;
    అయితే (*పాఠ్యం++) l++;
    తిరిగి l;
  }

  స్థిర గుర్తులేని పూర్ణాంకం
  encode_len (hb_codepoint_t unicode)
  {
    తిరిగి unicode < 0x10000 ? 1 : 2;
  }

  స్థిర codepoint_t *
  encode (codepoint_t *పాఠ్యం,
	  స్థిరరాశి codepoint_t *చివర,
	  hb_codepoint_t unicode)
  {
    ఒకవేళ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    ఒకవేళ (unicode < 0x10000u)
     *పాఠ్యం++ = unicode;
    లేకపోతే ఒకవేళ (చివర - పాఠ్యం >= 2)
    {
      unicode -= 0x10000u;
      *పాఠ్యం++ =  0xD800u + (unicode >> 10);
      *పాఠ్యం++ =  0xDC00u + (unicode & 0x03FFu);
    }
    తిరిగి పాఠ్యం;
  }
};

రకనిర్వచనం hb_utf16_xe_t<uint16_t> hb_utf16_t;
రకనిర్వచనం hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


మూస <రకం_పేరు TCodepoint, తార్కిక validate=నిజం>
నిర్మాణం hb_utf32_xe_t
{
  static_assert (పరిమాణం (TCodepoint) == 4, "");
  రకనిర్వచనం TCodepoint codepoint_t;
  స్థిర constexpr గుర్తులేని max_len = 1;

  స్థిర అంతఃవరుస స్థిరరాశి TCodepoint *
  next (స్థిరరాశి TCodepoint *పాఠ్యం,
	స్థిరరాశి TCodepoint *చివర HB_వాడని,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *పాఠ్యం++;
    ఒకవేళ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    తిరిగి పాఠ్యం;
  }

  స్థిర అంతఃవరుస స్థిరరాశి TCodepoint *
  prev (స్థిరరాశి TCodepoint *పాఠ్యం,
	స్థిరరాశి TCodepoint *ప్రారంభం HB_వాడని,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--పాఠ్యం;
    ఒకవేళ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    తిరిగి పాఠ్యం;
  }

  స్థిర గుర్తులేని పూర్ణాంకం
  అక్షరశ్రేణి_పొడవు (స్థిరరాశి TCodepoint *పాఠ్యం)
  {
    గుర్తులేని పూర్ణాంకం l = 0;
    అయితే (*పాఠ్యం++) l++;
    తిరిగి l;
  }

  స్థిర గుర్తులేని పూర్ణాంకం
  encode_len (hb_codepoint_t unicode HB_వాడని)
  {
    తిరిగి 1;
  }

  స్థిర codepoint_t *
  encode (codepoint_t *పాఠ్యం,
	  స్థిరరాశి codepoint_t *చివర HB_వాడని,
	  hb_codepoint_t unicode)
  {
    ఒకవేళ (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *పాఠ్యం++ = unicode;
    తిరిగి పాఠ్యం;
  }
};

రకనిర్వచనం hb_utf32_xe_t<uint32_t> hb_utf32_t;
రకనిర్వచనం hb_utf32_xe_t<uint32_t, అబద్ధం> hb_utf32_novalidate_t;


నిర్మాణం hb_latin1_t
{
  రకనిర్వచనం uint8_t codepoint_t;
  స్థిర constexpr గుర్తులేని max_len = 1;

  స్థిర అంతఃవరుస స్థిరరాశి codepoint_t *
  next (స్థిరరాశి codepoint_t *పాఠ్యం,
	స్థిరరాశి codepoint_t *చివర HB_వాడని,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_వాడని)
  {
    *unicode = *పాఠ్యం++;
    తిరిగి పాఠ్యం;
  }

  స్థిర అంతఃవరుస స్థిరరాశి codepoint_t *
  prev (స్థిరరాశి codepoint_t *పాఠ్యం,
	స్థిరరాశి codepoint_t *ప్రారంభం HB_వాడని,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_వాడని)
  {
    *unicode = *--పాఠ్యం;
    తిరిగి పాఠ్యం;
  }

  స్థిర గుర్తులేని పూర్ణాంకం
  అక్షరశ్రేణి_పొడవు (స్థిరరాశి codepoint_t *పాఠ్యం)
  {
    గుర్తులేని పూర్ణాంకం l = 0;
    అయితే (*పాఠ్యం++) l++;
    తిరిగి l;
  }

  స్థిర గుర్తులేని పూర్ణాంకం
  encode_len (hb_codepoint_t unicode HB_వాడని)
  {
    తిరిగి 1;
  }

  స్థిర codepoint_t *
  encode (codepoint_t *పాఠ్యం,
	  స్థిరరాశి codepoint_t *చివర HB_వాడని,
	  hb_codepoint_t unicode)
  {
    ఒకవేళ (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *పాఠ్యం++ = unicode;
    తిరిగి పాఠ్యం;
  }
};


నిర్మాణం hb_ascii_t
{
  రకనిర్వచనం uint8_t codepoint_t;
  స్థిర constexpr గుర్తులేని max_len = 1;

  స్థిర అంతఃవరుస స్థిరరాశి codepoint_t *
  next (స్థిరరాశి codepoint_t *పాఠ్యం,
	స్థిరరాశి codepoint_t *చివర HB_వాడని,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *పాఠ్యం++;
    ఒకవేళ (*unicode >= 0x0080u)
      *unicode = replacement;
    తిరిగి పాఠ్యం;
  }

  స్థిర అంతఃవరుస స్థిరరాశి codepoint_t *
  prev (స్థిరరాశి codepoint_t *పాఠ్యం,
	స్థిరరాశి codepoint_t *ప్రారంభం HB_వాడని,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--పాఠ్యం;
    ఒకవేళ (*unicode >= 0x0080u)
      *unicode = replacement;
    తిరిగి పాఠ్యం;
  }

  స్థిర గుర్తులేని పూర్ణాంకం
  అక్షరశ్రేణి_పొడవు (స్థిరరాశి codepoint_t *పాఠ్యం)
  {
    గుర్తులేని పూర్ణాంకం l = 0;
    అయితే (*పాఠ్యం++) l++;
    తిరిగి l;
  }

  స్థిర గుర్తులేని పూర్ణాంకం
  encode_len (hb_codepoint_t unicode HB_వాడని)
  {
    తిరిగి 1;
  }

  స్థిర codepoint_t *
  encode (codepoint_t *పాఠ్యం,
	  స్థిరరాశి codepoint_t *చివర HB_వాడని,
	  hb_codepoint_t unicode)
  {
    ఒకవేళ (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *పాఠ్యం++ = unicode;
    తిరిగి పాఠ్యం;
  }
};

మూస <రకం_పేరు utf_t>
స్థిర అంతఃవరుస స్థిరరాశి రకం_పేరు utf_t::codepoint_t *
hb_utf_స్థానభ్రంశం_గమ్యం_సూచిక (స్థిరరాశి రకం_పేరు utf_t::codepoint_t *ప్రారంభం,
			  స్థిరరాశి రకం_పేరు utf_t::codepoint_t *పాఠ్యం,
			  గుర్తులేని పాఠ్యం_len,
			  గుర్తుగల స్థానభ్రంశం)
{
  hb_codepoint_t unicode;

  అయితే (స్థానభ్రంశం-- > 0)
    ప్రారంభం = utf_t::next (ప్రారంభం,
			 పాఠ్యం + పాఠ్యం_len,
			 &unicode,
			 HB_బఫరు_REPLACEMENT_CODEPOINT_DEFAULT);

  అయితే (స్థానభ్రంశం++ < 0)
    ప్రారంభం = utf_t::prev (ప్రారంభం,
			 పాఠ్యం,
			 &unicode,
			 HB_బఫరు_REPLACEMENT_CODEPOINT_DEFAULT);

  తిరిగి ప్రారంభం;
}


#అయితే_ముగింపు /* HB_UTF_HH */
