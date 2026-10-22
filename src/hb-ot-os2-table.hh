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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-os2-unicode-ranges.hh"
#অন্তর্ভাব "hb-ot-var-mvar-table.hh"

#অন্তর্ভাব "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


নামাকাশ OT {

কাঠামো OS2_V1_লেজ
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো OS2_V2_লেজ
{
  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত sxHeight || sCapHeight; }

  ধ্রুবক OS2_V2_লেজ * operator -> () ধ্রুবক { ফেরত এই; }
  OS2_V2_লেজ * operator -> () { ফেরত এই; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  সর্বজনীন:
  DEFINE_আকার_STATIC (10);
};

কাঠামো OS2_V5_লেজ
{
  অন্তঃরেখ যৌক্তিক get_optical_আকার (unsigned পূর্ণসংখ্যা *lower, unsigned পূর্ণসংখ্যা *upper) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা lower_optical_আকার = us_Lower_Optical_Point_আকার;
    unsigned পূর্ণসংখ্যা upper_optical_আকার = us_Upper_Optical_Point_আকার;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    যদি (lower_optical_আকার < upper_optical_আকার &&
	lower_optical_আকার >= 1 && lower_optical_আকার <= 0xFFFE &&
	upper_optical_আকার >= 2 && upper_optical_আকার <= 0xFFFF)
    {
      *lower = lower_optical_আকার;
      *upper = upper_optical_আকার;
      ফেরত সত্য;
    }
    ফেরত মিথ্যা;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBUINT16	us_Lower_Optical_Point_আকার;
  HBUINT16	us_Upper_Optical_Point_আকার;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো OS2
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত usWeightClass || usWidthClass || us_First_Char_সূচক || us_Last_Char_সূচক; }

  ধ্রুবক OS2_V1_লেজ &v1 () ধ্রুবক { ফেরত version >= 1 ? v1X : Null (OS2_V1_লেজ); }
  ধ্রুবক OS2_V2_লেজ &v2 () ধ্রুবক { ফেরত version >= 2 ? v2X : Null (OS2_V2_লেজ); }
  ধ্রুবক OS2_V5_লেজ &v5 () ধ্রুবক { ফেরত version >= 5 ? v5X : Null (OS2_V5_লেজ); }

  গণনা selection_flag_t {
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

  যৌক্তিক        is_italic () ধ্রুবক { ফেরত fsSelection & ITALIC; }
  যৌক্তিক       is_oblique () ধ্রুবক { ফেরত fsSelection & OBLIQUE; }
  যৌক্তিক use_typo_metrics () ধ্রুবক { ফেরত fsSelection & USE_TYPO_METRICS; }

  গণনা width_class_t {
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

  দশমিক get_width () ধ্রুবক
  {
    নির্বাচন (usWidthClass) {
    ক্ষেত্র FWIDTH_ULTRA_CONDENSED:ফেরত 50.f;
    ক্ষেত্র FWIDTH_EXTRA_CONDENSED:ফেরত 62.5f;
    ক্ষেত্র FWIDTH_CONDENSED:	ফেরত 75.f;
    ক্ষেত্র FWIDTH_SEMI_CONDENSED:	ফেরত 87.5f;
    পূর্বনির্ধারিত:
    ক্ষেত্র FWIDTH_NORMAL:		ফেরত 100.f;
    ক্ষেত্র FWIDTH_SEMI_EXPANDED:	ফেরত 112.5f;
    ক্ষেত্র FWIDTH_EXPANDED:	ফেরত 125.f;
    ক্ষেত্র FWIDTH_EXTRA_EXPANDED:	ফেরত 150.f;
    ক্ষেত্র FWIDTH_ULTRA_EXPANDED:	ফেরত 200.f;
    }
  }

  দশমিক map_wdth_to_widthclass(দশমিক width) ধ্রুবক
  {
    যদি (width < 50) ফেরত 1.0f;
    যদি (width > 200) ফেরত 9.0f;

    দশমিক ratio = (width - 50) / 12.5f;
    পূর্ণসংখ্যা a = (পূর্ণসংখ্যা) floorf (ratio);
    পূর্ণসংখ্যা b = (পূর্ণসংখ্যা) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    যদি (b <= 6) // 50-125
    {
      যদি (a == b) ফেরত a + 1.0f;
    }
    অন্যথা যদি (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    অন্যথা যদি (b == 8)
    {
      যদি (a == b) ফেরত 8.0f; // 150
      a = 6;
    }
    অন্যথা
    {
      যদি (a == b && a == 12) ফেরত 9.0f; //200
      b = 12;
      a = 8;
    }

    দশমিক va = 50 + a * 12.5f;
    দশমিক vb = 50 + b * 12.5f;

    দশমিক ret =  a + (width - va) / (vb - va);
    যদি (a <= 6) ret += 1.0f;
    ফেরত ret;
  }

  স্থির unsigned calc_avg_char_width (ধ্রুবক hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>>& hmtx_map)
  {
    unsigned অঙ্ক = 0;
    unsigned যোগ_width = 0;
    জন্য (ধ্রুবক auto& _ : hmtx_map.মানসমূহ_ref ())
    {
      unsigned width = _.first;
      যদি (width)
      {
        যোগ_width += width;
        অঙ্ক++;
      }
    }

    ফেরত অঙ্ক ? (unsigned) roundf ((দ্বিগুণ) যোগ_width / (দ্বিগুণ) অঙ্ক) : 0;
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    OS2 *os2_মৌলিক = c->serializer->embed (এই);
    যদি (unlikely (!os2_মৌলিক)) return_trace (মিথ্যা);

#ifndef HB_NO_VAR
    যদি (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_মৌলিক;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP,         s_Typo_সারি_Gap);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_আকার,         ySubscriptXSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_আকার,         ySubscriptYSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET,       ySubscriptXOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET,       ySubscriptYOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_আকার,       ySuperscriptXSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_আকার,       ySuperscriptYSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET,     ySuperscriptXOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET,     ySuperscriptYOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_STRIKEOUT_আকার,              y_Strikeout_আকার);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_STRIKEOUT_OFFSET,            yStrikeoutPosition);

      যদি (os2_মৌলিক->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2_V2_লেজ &> (os2_মৌলিক->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      যদি (!c->serializer->check_assign (os2_মৌলিক->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (মিথ্যা);
    }
#endif

    Triple *axis_range;
    যদি (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      unsigned weight_class = static_cast<unsigned> (roundf (hb_clamp (axis_range->মধ্য, 1.0f, 1000.0f)));
      যদি (os2_মৌলিক->usWeightClass != weight_class)
        os2_মৌলিক->usWeightClass = weight_class;
    }

    যদি (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      unsigned width_class = static_cast<unsigned> (roundf (map_wdth_to_widthclass (axis_range->মধ্য)));
      যদি (os2_মৌলিক->usWidthClass != width_class)
        os2_মৌলিক->usWidthClass = width_class;
    }

    os2_মৌলিক->us_First_Char_সূচক = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_মৌলিক->us_Last_Char_সূচক  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (সত্য);

    _update_unicode_ranges (&c->plan->unicodes, os2_মৌলিক->ulUnicodeRange);

    return_trace (সত্য);
  }

  শূন্য _update_unicode_ranges (ধ্রুবক hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ধ্রুবক
  {
    HBUINT32 newBits[4];
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    জন্য (auto cp : *codepoints)
    {
      unsigned পূর্ণসংখ্যা bit = _hb_ot_os2_get_unicode_range_bit (cp);
      যদি (bit < 128)
      {
	unsigned পূর্ণসংখ্যা block = bit / 32;
	unsigned পূর্ণসংখ্যা bit_in_block = bit % 32;
	unsigned পূর্ণসংখ্যা mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      যদি (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  গণনা font_page_t
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
  font_page_t get_font_page () ধ্রুবক
  { ফেরত (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_আকার () ধ্রুবক
  {
    size_t ফলাফল = min_আকার;
    যদি (version >= 1) ফলাফল = hb_unsigned_add_saturate (ফলাফল, v1X.get_আকার ());
    যদি (version >= 2) ফলাফল = hb_unsigned_add_saturate (ফলাফল, v2X.get_আকার ());
    যদি (version >= 5) ফলাফল = hb_unsigned_add_saturate (ফলাফল, v5X.get_আকার ());
    ফেরত ফলাফল;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই))) return_trace (মিথ্যা);
    hb_barrier ();
    যদি (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (মিথ্যা);
    যদি (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (মিথ্যা);
    যদি (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  সর্বজনীন:
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
  HBINT16	y_Strikeout_আকার;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  Tag		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_First_Char_সূচক;
  HBUINT16	us_Last_Char_সূচক;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_সারি_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_লেজ	v1X;
  OS2_V2_লেজ	v2X;
  OS2_V5_লেজ	v5X;
  সর্বজনীন:
  DEFINE_আকার_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
