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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-os2-unicode-ranges.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-mvar-table.hh"

#ಅನ್ತರ್ಭಾವ "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


ನಾಮಾಕಾಶ OT {

ಸಂರಚನಾ OS2V1Tail
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ OS2V2Tail
{
  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ sxHeight || sCapHeight; }

  ಧ್ರುವ OS2V2Tail * operator -> () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್; }
  OS2V2Tail * operator -> () { ಪ್ರತ್ಯಾಗಮ ಏತತ್; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (10);
};

ಸಂರಚನಾ OS2V5Tail
{
  ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ get_optical_size (unsigned ಪೂರ್ಣಾಙ್ಕ *lower, unsigned ಪೂರ್ಣಾಙ್ಕ *upper) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ lower_optical_size = usLowerOpticalPointSize;
    unsigned ಪೂರ್ಣಾಙ್ಕ upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    ಯದಿ (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16	usLowerOpticalPointSize;
  HBUINT16	usUpperOpticalPointSize;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ OS2
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  ಧ್ರುವ OS2V1Tail &v1 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version >= 1 ? v1X : Null (OS2V1Tail); }
  ಧ್ರುವ OS2V2Tail &v2 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version >= 2 ? v2X : Null (OS2V2Tail); }
  ಧ್ರುವ OS2V5Tail &v5 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version >= 5 ? v5X : Null (OS2V5Tail); }

  ಗಣನಾ selection_flag_t {
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

  ತಾರ್ಕಿಕ        is_italic () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ fsSelection & ITALIC; }
  ತಾರ್ಕಿಕ       is_oblique () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ fsSelection & OBLIQUE; }
  ತಾರ್ಕಿಕ use_typo_metrics () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ fsSelection & USE_TYPO_METRICS; }

  ಗಣನಾ width_class_t {
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

  ಭಿನ್ನ get_width () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (usWidthClass) {
    ಪಕ್ಷ FWIDTH_ULTRA_CONDENSED:ಪ್ರತ್ಯಾಗಮ 50.f;
    ಪಕ್ಷ FWIDTH_EXTRA_CONDENSED:ಪ್ರತ್ಯಾಗಮ 62.5f;
    ಪಕ್ಷ FWIDTH_CONDENSED:	ಪ್ರತ್ಯಾಗಮ 75.f;
    ಪಕ್ಷ FWIDTH_SEMI_CONDENSED:	ಪ್ರತ್ಯಾಗಮ 87.5f;
    ಸ್ವಭಾವ:
    ಪಕ್ಷ FWIDTH_NORMAL:		ಪ್ರತ್ಯಾಗಮ 100.f;
    ಪಕ್ಷ FWIDTH_SEMI_EXPANDED:	ಪ್ರತ್ಯಾಗಮ 112.5f;
    ಪಕ್ಷ FWIDTH_EXPANDED:	ಪ್ರತ್ಯಾಗಮ 125.f;
    ಪಕ್ಷ FWIDTH_EXTRA_EXPANDED:	ಪ್ರತ್ಯಾಗಮ 150.f;
    ಪಕ್ಷ FWIDTH_ULTRA_EXPANDED:	ಪ್ರತ್ಯಾಗಮ 200.f;
    }
  }

  ಭಿನ್ನ map_wdth_to_widthclass(ಭಿನ್ನ width) ಧ್ರುವ
  {
    ಯದಿ (width < 50) ಪ್ರತ್ಯಾಗಮ 1.0f;
    ಯದಿ (width > 200) ಪ್ರತ್ಯಾಗಮ 9.0f;

    ಭಿನ್ನ ratio = (width - 50) / 12.5f;
    ಪೂರ್ಣಾಙ್ಕ a = (ಪೂರ್ಣಾಙ್ಕ) floorf (ratio);
    ಪೂರ್ಣಾಙ್ಕ b = (ಪೂರ್ಣಾಙ್ಕ) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    ಯದಿ (b <= 6) // 50-125
    {
      ಯದಿ (a == b) ಪ್ರತ್ಯಾಗಮ a + 1.0f;
    }
    ಅನ್ಯಥಾ ಯದಿ (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    ಅನ್ಯಥಾ ಯದಿ (b == 8)
    {
      ಯದಿ (a == b) ಪ್ರತ್ಯಾಗಮ 8.0f; // 150
      a = 6;
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (a == b && a == 12) ಪ್ರತ್ಯಾಗಮ 9.0f; //200
      b = 12;
      a = 8;
    }

    ಭಿನ್ನ va = 50 + a * 12.5f;
    ಭಿನ್ನ vb = 50 + b * 12.5f;

    ಭಿನ್ನ ret =  a + (width - va) / (vb - va);
    ಯದಿ (a <= 6) ret += 1.0f;
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಸ್ಥಿರ unsigned calc_avg_char_width (ಧ್ರುವ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>>& hmtx_map)
  {
    unsigned ಅಂಕೆ = 0;
    unsigned total_width = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : hmtx_map.ಮೌಲ್ಯಗಳು_ref ())
    {
      unsigned width = _.first;
      ಯದಿ (width)
      {
        total_width += width;
        ಅಂಕೆ++;
      }
    }

    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ ? (unsigned) roundf ((ದ್ವಿಗುಣ) total_width / (ದ್ವಿಗುಣ) ಅಂಕೆ) : 0;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    OS2 *os2_ಅವಿಭಾಜ್ಯ = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!os2_ಅವಿಭಾಜ್ಯ)) return_trace (ಅಸತ್ಯ);

#ifndef HB_NO_VAR
    ಯದಿ (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_ಅವಿಭಾಜ್ಯ;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ಸಾಲು_GAP,         s_Typo_ಸಾಲು_Gap);
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

      ಯದಿ (os2_ಅವಿಭಾಜ್ಯ->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2V2Tail &> (os2_ಅವಿಭಾಜ್ಯ->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      ಯದಿ (!c->serializer->check_assign (os2_ಅವಿಭಾಜ್ಯ->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (ಅಸತ್ಯ);
    }
#endif

    Triple *axis_range;
    ಯದಿ (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      unsigned weight_class = static_cast<unsigned> (roundf (hb_clamp (axis_range->ಮಧ್ಯ, 1.0f, 1000.0f)));
      ಯದಿ (os2_ಅವಿಭಾಜ್ಯ->usWeightClass != weight_class)
        os2_ಅವಿಭಾಜ್ಯ->usWeightClass = weight_class;
    }

    ಯದಿ (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      unsigned width_class = static_cast<unsigned> (roundf (map_wdth_to_widthclass (axis_range->ಮಧ್ಯ)));
      ಯದಿ (os2_ಅವಿಭಾಜ್ಯ->usWidthClass != width_class)
        os2_ಅವಿಭಾಜ್ಯ->usWidthClass = width_class;
    }

    os2_ಅವಿಭಾಜ್ಯ->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_ಅವಿಭಾಜ್ಯ->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    ಯದಿ (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (ಸತ್ಯ);

    _update_unicode_ranges (&c->plan->unicodes, os2_ಅವಿಭಾಜ್ಯ->ulUnicodeRange);

    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ _update_unicode_ranges (ಧ್ರುವ hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ಧ್ರುವ
  {
    HBUINT32 newBits[4];
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    ಪ್ರತಿ (auto cp : *codepoints)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ bit = _hb_ot_os2_get_unicode_range_bit (cp);
      ಯದಿ (bit < 128)
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ block = bit / 32;
	unsigned ಪೂರ್ಣಾಙ್ಕ bit_in_block = bit % 32;
	unsigned ಪೂರ್ಣಾಙ್ಕ mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      ಯದಿ (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  ಗಣನಾ font_page_t
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
  font_page_t get_font_page () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_size () ಧ್ರುವ
  {
    size_t result = min_size;
    ಯದಿ (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    ಯದಿ (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    ಯದಿ (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    ಪ್ರತ್ಯಾಗಮ result;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಯದಿ (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ಸಾರ್ವಜನಿಕ:
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
  HBINT16	s_Typo_ಸಾಲು_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2V1Tail	v1X;
  OS2V2Tail	v2X;
  OS2V5Tail	v5X;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
