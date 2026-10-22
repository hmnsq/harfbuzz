/*
 * Copyright © 2011,2012  Google, Inc.
 * Copyright © 2018  Ebrahim Byagowi
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

#ifndef HB_OT_OS2_TABLE_HH
#define HB_OT_OS2_TABLE_HH

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-os2-unicode-ranges.hh"
#અન્તર્ભાવ "hb-ot-var-mvar-table.hh"

#અન્તર્ભાવ "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


નામાકાશ OT {

સંરચના OS2V1Tail
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના OS2V2Tail
{
  તાર્કિક has_data () અચલ { પાછા sxHeight || sCapHeight; }

  અચલ OS2V2Tail * operator -> () અચલ { પાછા આ; }
  OS2V2Tail * operator -> () { પાછા આ; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  જાહેર:
  DEFINE_SIZE_STATIC (10);
};

સંરચના OS2V5Tail
{
  અંતઃરેખ તાર્કિક get_optical_size (unsigned પૂર્ણાંક *lower, unsigned પૂર્ણાંક *upper) અચલ
  {
    unsigned પૂર્ણાંક lower_optical_size = usLowerOpticalPointSize;
    unsigned પૂર્ણાંક upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    જો (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      પાછા સાચું;
    }
    પાછા ખોટું;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBUINT16	usLowerOpticalPointSize;
  HBUINT16	usUpperOpticalPointSize;
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના OS2
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  તાર્કિક has_data () અચલ { પાછા usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  અચલ OS2V1Tail &v1 () અચલ { પાછા version >= 1 ? v1X : Null (OS2V1Tail); }
  અચલ OS2V2Tail &v2 () અચલ { પાછા version >= 2 ? v2X : Null (OS2V2Tail); }
  અચલ OS2V5Tail &v5 () અચલ { પાછા version >= 5 ? v5X : Null (OS2V5Tail); }

  ગણના selection_flag_t {
    ITALIC		= 1u<<0,
    UNDERSCORE		= 1u<<1,
    NEGATIVE		= 1u<<2,
    OUTLINED		= 1u<<3,
    STRIKEOUT		= 1u<<4,
    BOLD		= 1u<<5,
    REGULAR		= 1u<<6,
    USE_TYPO_METRICS	= 1u<<7,
    WWS			= 1u<<8,
    OBLIQUE		= 1u<<9
  };

  તાર્કિક        is_italic () અચલ { પાછા fsSelection & ITALIC; }
  તાર્કિક       is_oblique () અચલ { પાછા fsSelection & OBLIQUE; }
  તાર્કિક use_typo_metrics () અચલ { પાછા fsSelection & USE_TYPO_METRICS; }

  ગણના width_class_t {
    FWIDTH_ULTRA_CONDENSED	= 1, /* 50% */
    FWIDTH_EXTRA_CONDENSED	= 2, /* 62.5% */
    FWIDTH_CONDENSED		= 3, /* 75% */
    FWIDTH_SEMI_CONDENSED	= 4, /* 87.5% */
    FWIDTH_NORMAL		= 5, /* 100% */
    FWIDTH_SEMI_EXPANDED	= 6, /* 112.5% */
    FWIDTH_EXPANDED		= 7, /* 125% */
    FWIDTH_EXTRA_EXPANDED	= 8, /* 150% */
    FWIDTH_ULTRA_EXPANDED	= 9  /* 200% */
  };

  દશાંશ get_width () અચલ
  {
    પસંદગી (usWidthClass) {
    સ્થિતિ FWIDTH_ULTRA_CONDENSED:પાછા 50.f;
    સ્થિતિ FWIDTH_EXTRA_CONDENSED:પાછા 62.5f;
    સ્થિતિ FWIDTH_CONDENSED:	પાછા 75.f;
    સ્થિતિ FWIDTH_SEMI_CONDENSED:	પાછા 87.5f;
    મૂળભૂત:
    સ્થિતિ FWIDTH_NORMAL:		પાછા 100.f;
    સ્થિતિ FWIDTH_SEMI_EXPANDED:	પાછા 112.5f;
    સ્થિતિ FWIDTH_EXPANDED:	પાછા 125.f;
    સ્થિતિ FWIDTH_EXTRA_EXPANDED:	પાછા 150.f;
    સ્થિતિ FWIDTH_ULTRA_EXPANDED:	પાછા 200.f;
    }
  }

  દશાંશ map_wdth_to_widthclass(દશાંશ width) અચલ
  {
    જો (width < 50) પાછા 1.0f;
    જો (width > 200) પાછા 9.0f;

    દશાંશ ratio = (width - 50) / 12.5f;
    પૂર્ણાંક a = (પૂર્ણાંક) floorf (ratio);
    પૂર્ણાંક b = (પૂર્ણાંક) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    જો (b <= 6) // 50-125
    {
      જો (a == b) પાછા a + 1.0f;
    }
    નહીંતર જો (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    નહીંતર જો (b == 8)
    {
      જો (a == b) પાછા 8.0f; // 150
      a = 6;
    }
    નહીંતર
    {
      જો (a == b && a == 12) પાછા 9.0f; //200
      b = 12;
      a = 8;
    }

    દશાંશ va = 50 + a * 12.5f;
    દશાંશ vb = 50 + b * 12.5f;

    દશાંશ ret =  a + (width - va) / (vb - va);
    જો (a <= 6) ret += 1.0f;
    પાછા ret;
  }

  સ્થિર unsigned calc_avg_char_width (અચલ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, પૂર્ણાંક>>& hmtx_map)
  {
    unsigned અંક = 0;
    unsigned total_width = 0;
    માટે (અચલ auto& _ : hmtx_map.કિંમતો_ref ())
    {
      unsigned width = _.first;
      જો (width)
      {
        total_width += width;
        અંક++;
      }
    }

    પાછા અંક ? (unsigned) roundf ((દ્વિગુણ) total_width / (દ્વિગુણ) અંક) : 0;
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    OS2 *os2_અવિભાજ્ય = c->serializer->embed (આ);
    જો (unlikely (!os2_અવિભાજ્ય)) return_trace (ખોટું);

#ifndef HB_NO_VAR
    જો (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_અવિભાજ્ય;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP,         s_Typo_લીટી_Gap);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE,         ySubscriptXSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE,         ySubscriptYSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET,       ySubscriptXOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET,       ySubscriptYOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE,       ySuperscriptXSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE,       ySuperscriptYSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET,     ySuperscriptXOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET,     ySuperscriptYOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_STRIKEOUT_SIZE,              yStrikeoutSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_STRIKEOUT_OFFSET,            yStrikeoutPosition);

      જો (os2_અવિભાજ્ય->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2V2Tail &> (os2_અવિભાજ્ય->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      જો (!c->serializer->check_assign (os2_અવિભાજ્ય->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (ખોટું);
    }
#endif

    Triple *axis_range;
    જો (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      unsigned weight_class = static_cast<unsigned> (roundf (hb_clamp (axis_range->મધ્ય, 1.0f, 1000.0f)));
      જો (os2_અવિભાજ્ય->usWeightClass != weight_class)
        os2_અવિભાજ્ય->usWeightClass = weight_class;
    }

    જો (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      unsigned width_class = static_cast<unsigned> (roundf (map_wdth_to_widthclass (axis_range->મધ્ય)));
      જો (os2_અવિભાજ્ય->usWidthClass != width_class)
        os2_અવિભાજ્ય->usWidthClass = width_class;
    }

    os2_અવિભાજ્ય->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_અવિભાજ્ય->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    જો (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (સાચું);

    _update_unicode_ranges (&c->plan->unicodes, os2_અવિભાજ્ય->ulUnicodeRange);

    return_trace (સાચું);
  }

  શૂન્ય _update_unicode_ranges (અચલ hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) અચલ
  {
    HBUINT32 newBits[4];
    માટે (unsigned પૂર્ણાંક i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    માટે (auto cp : *codepoints)
    {
      unsigned પૂર્ણાંક bit = _hb_ot_os2_get_unicode_range_bit (cp);
      જો (bit < 128)
      {
	unsigned પૂર્ણાંક block = bit / 32;
	unsigned પૂર્ણાંક bit_in_block = bit % 32;
	unsigned પૂર્ણાંક mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      જો (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    માટે (unsigned પૂર્ણાંક i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  ગણના font_page_t
  {
    FONT_PAGE_NONE		= 0,
    FONT_PAGE_HEBREW		= 0xB100, /* Hebrew Windows 3.1 font page */
    FONT_PAGE_SIMP_ARABIC	= 0xB200, /* Simplified Arabic Windows 3.1 font page */
    FONT_PAGE_TRAD_ARABIC	= 0xB300, /* Traditional Arabic Windows 3.1 font page */
    FONT_PAGE_OEM_ARABIC	= 0xB400, /* OEM Arabic Windows 3.1 font page */
    FONT_PAGE_SIMP_FARSI	= 0xBA00, /* Simplified Farsi Windows 3.1 font page */
    FONT_PAGE_TRAD_FARSI	= 0xBB00, /* Traditional Farsi Windows 3.1 font page */
    FONT_PAGE_THAI		= 0xDE00  /* Thai Windows 3.1 font page */
  };
  font_page_t get_font_page () અચલ
  { પાછા (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_size () અચલ
  {
    size_t result = min_size;
    જો (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    જો (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    જો (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    પાછા result;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ))) return_trace (ખોટું);
    hb_barrier ();
    જો (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (ખોટું);
    જો (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (ખોટું);
    જો (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (ખોટું);
    return_trace (સાચું);
  }

  જાહેર:
  HBUINT16	version;
  HBINT16	xAvgCharWidth;
  HBUINT16	usWeightClass;
  HBUINT16	usWidthClass;
  HBUINT16	fsType;
  HBINT16	ySubscriptXSize;
  HBINT16	ySubscriptYSize;
  HBINT16	ySubscriptXOffset;
  HBINT16	ySubscriptYOffset;
  HBINT16	ySuperscriptXSize;
  HBINT16	ySuperscriptYSize;
  HBINT16	ySuperscriptXOffset;
  HBINT16	ySuperscriptYOffset;
  HBINT16	yStrikeoutSize;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  Tag		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	usFirstCharIndex;
  HBUINT16	usLastCharIndex;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_લીટી_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2V1Tail	v1X;
  OS2V2Tail	v2X;
  OS2V5Tail	v5X;
  જાહેર:
  DEFINE_SIZE_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
