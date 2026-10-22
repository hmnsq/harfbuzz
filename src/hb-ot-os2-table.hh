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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-os2-unicode-ranges.hh"
#அந்தர்பாவ "hb-ot-var-mvar-table.hh"

#அந்தர்பாவ "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


நாமாகாஶ OT {

கட்டமைப்பு OS2V1Tail
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு OS2V2Tail
{
  தருக்க has_data () மாறிலி { திரும்பு sxHeight || sCapHeight; }

  மாறிலி OS2V2Tail * operator -> () மாறிலி { திரும்பு இது; }
  OS2V2Tail * operator -> () { திரும்பு இது; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  பொது:
  DEFINE_SIZE_STATIC (10);
};

கட்டமைப்பு OS2V5Tail
{
  உள்வரி தருக்க get_optical_size (unsigned முழு *lower, unsigned முழு *upper) மாறிலி
  {
    unsigned முழு lower_optical_size = usLowerOpticalPointSize;
    unsigned முழு upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    எனில் (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      திரும்பு உண்மை;
    }
    திரும்பு பொய்;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBUINT16	usLowerOpticalPointSize;
  HBUINT16	usUpperOpticalPointSize;
  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு OS2
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  தருக்க has_data () மாறிலி { திரும்பு usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  மாறிலி OS2V1Tail &v1 () மாறிலி { திரும்பு version >= 1 ? v1X : Null (OS2V1Tail); }
  மாறிலி OS2V2Tail &v2 () மாறிலி { திரும்பு version >= 2 ? v2X : Null (OS2V2Tail); }
  மாறிலி OS2V5Tail &v5 () மாறிலி { திரும்பு version >= 5 ? v5X : Null (OS2V5Tail); }

  எண்ணிக்கை selection_flag_t {
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

  தருக்க        is_italic () மாறிலி { திரும்பு fsSelection & ITALIC; }
  தருக்க       is_oblique () மாறிலி { திரும்பு fsSelection & OBLIQUE; }
  தருக்க use_typo_metrics () மாறிலி { திரும்பு fsSelection & USE_TYPO_METRICS; }

  எண்ணிக்கை width_class_t {
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

  தசம get_width () மாறிலி
  {
    தேர்வு (usWidthClass) {
    நிலை FWIDTH_ULTRA_CONDENSED:திரும்பு 50.f;
    நிலை FWIDTH_EXTRA_CONDENSED:திரும்பு 62.5f;
    நிலை FWIDTH_CONDENSED:	திரும்பு 75.f;
    நிலை FWIDTH_SEMI_CONDENSED:	திரும்பு 87.5f;
    இயல்பு:
    நிலை FWIDTH_NORMAL:		திரும்பு 100.f;
    நிலை FWIDTH_SEMI_EXPANDED:	திரும்பு 112.5f;
    நிலை FWIDTH_EXPANDED:	திரும்பு 125.f;
    நிலை FWIDTH_EXTRA_EXPANDED:	திரும்பு 150.f;
    நிலை FWIDTH_ULTRA_EXPANDED:	திரும்பு 200.f;
    }
  }

  தசம map_wdth_to_widthclass(தசம width) மாறிலி
  {
    எனில் (width < 50) திரும்பு 1.0f;
    எனில் (width > 200) திரும்பு 9.0f;

    தசம ratio = (width - 50) / 12.5f;
    முழு a = (முழு) floorf (ratio);
    முழு b = (முழு) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    எனில் (b <= 6) // 50-125
    {
      எனில் (a == b) திரும்பு a + 1.0f;
    }
    இல்லையெனில் எனில் (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    இல்லையெனில் எனில் (b == 8)
    {
      எனில் (a == b) திரும்பு 8.0f; // 150
      a = 6;
    }
    இல்லையெனில்
    {
      எனில் (a == b && a == 12) திரும்பு 9.0f; //200
      b = 12;
      a = 8;
    }

    தசம va = 50 + a * 12.5f;
    தசம vb = 50 + b * 12.5f;

    தசம ret =  a + (width - va) / (vb - va);
    எனில் (a <= 6) ret += 1.0f;
    திரும்பு ret;
  }

  நிலையான unsigned calc_avg_char_width (மாறிலி hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, முழு>>& hmtx_map)
  {
    unsigned இலக்கம் = 0;
    unsigned total_width = 0;
    க்கு (மாறிலி auto& _ : hmtx_map.மதிப்புகள்_ref ())
    {
      unsigned width = _.first;
      எனில் (width)
      {
        total_width += width;
        இலக்கம்++;
      }
    }

    திரும்பு இலக்கம் ? (unsigned) roundf ((இரட்டை) total_width / (இரட்டை) இலக்கம்) : 0;
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    OS2 *os2_பகாஎண் = c->serializer->embed (இது);
    எனில் (unlikely (!os2_பகாஎண்)) return_trace (பொய்);

#ifndef HB_NO_VAR
    எனில் (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_பகாஎண்;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP,         s_Typo_வரி_Gap);
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

      எனில் (os2_பகாஎண்->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2V2Tail &> (os2_பகாஎண்->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      எனில் (!c->serializer->check_assign (os2_பகாஎண்->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (பொய்);
    }
#endif

    Triple *axis_range;
    எனில் (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      unsigned weight_class = static_cast<unsigned> (roundf (hb_clamp (axis_range->நடு, 1.0f, 1000.0f)));
      எனில் (os2_பகாஎண்->usWeightClass != weight_class)
        os2_பகாஎண்->usWeightClass = weight_class;
    }

    எனில் (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      unsigned width_class = static_cast<unsigned> (roundf (map_wdth_to_widthclass (axis_range->நடு)));
      எனில் (os2_பகாஎண்->usWidthClass != width_class)
        os2_பகாஎண்->usWidthClass = width_class;
    }

    os2_பகாஎண்->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_பகாஎண்->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    எனில் (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (உண்மை);

    _update_unicode_ranges (&c->plan->unicodes, os2_பகாஎண்->ulUnicodeRange);

    return_trace (உண்மை);
  }

  வெறுமை _update_unicode_ranges (மாறிலி hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) மாறிலி
  {
    HBUINT32 newBits[4];
    க்கு (unsigned முழு i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    க்கு (auto cp : *codepoints)
    {
      unsigned முழு bit = _hb_ot_os2_get_unicode_range_bit (cp);
      எனில் (bit < 128)
      {
	unsigned முழு block = bit / 32;
	unsigned முழு bit_in_block = bit % 32;
	unsigned முழு mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      எனில் (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    க்கு (unsigned முழு i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  எண்ணிக்கை font_page_t
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
  font_page_t get_font_page () மாறிலி
  { திரும்பு (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_size () மாறிலி
  {
    size_t result = min_size;
    எனில் (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    எனில் (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    எனில் (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    திரும்பு result;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது))) return_trace (பொய்);
    hb_barrier ();
    எனில் (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (பொய்);
    எனில் (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (பொய்);
    எனில் (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (பொய்);
    return_trace (உண்மை);
  }

  பொது:
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
  HBINT16	s_Typo_வரி_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2V1Tail	v1X;
  OS2V2Tail	v2X;
  OS2V5Tail	v5X;
  பொது:
  DEFINE_SIZE_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
