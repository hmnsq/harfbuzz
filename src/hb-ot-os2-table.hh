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

#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-ot-os2-unicode-ranges.hh"
#অন্তর্ভাৱ "hb-ot-var-mvar-table.hh"

#অন্তর্ভাৱ "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


নামাকাশ OT {

সংরচনা OS2V1Tail
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্ৱজনিক:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা OS2V2Tail
{
  তার্কিক has_data () ধ্রুৱ { ঘূৰাই sxHeight || sCapHeight; }

  ধ্রুৱ OS2V2Tail * operator -> () ধ্রুৱ { ঘূৰাই এতত্; }
  OS2V2Tail * operator -> () { ঘূৰাই এতত্; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্ৱজনিক:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (10);
};

সংরচনা OS2V5Tail
{
  অন্তঃস্থ তার্কিক get_optical_size (unsigned পূর্ণাঙ্ক *lower, unsigned পূর্ণাঙ্ক *upper) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক lower_optical_size = usLowerOpticalPointSize;
    unsigned পূর্ণাঙ্ক upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    যদি (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্ৱজনিক:
  HBUINT16	usLowerOpticalPointSize;
  HBUINT16	usUpperOpticalPointSize;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা OS2
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  ধ্রুৱ OS2V1Tail &v1 () ধ্রুৱ { ঘূৰাই version >= 1 ? v1X : Null (OS2V1Tail); }
  ধ্রুৱ OS2V2Tail &v2 () ধ্রুৱ { ঘূৰাই version >= 2 ? v2X : Null (OS2V2Tail); }
  ধ্রুৱ OS2V5Tail &v5 () ধ্রুৱ { ঘূৰাই version >= 5 ? v5X : Null (OS2V5Tail); }

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

  তার্কিক        is_italic () ধ্রুৱ { ঘূৰাই fsSelection & ITALIC; }
  তার্কিক       is_oblique () ধ্রুৱ { ঘূৰাই fsSelection & OBLIQUE; }
  তার্কিক use_typo_metrics () ধ্রুৱ { ঘূৰাই fsSelection & USE_TYPO_METRICS; }

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

  ভিন্ন get_width () ধ্রুৱ
  {
    ৱিকল্প (usWidthClass) {
    পক্ষ FWIDTH_ULTRA_CONDENSED:ঘূৰাই 50.f;
    পক্ষ FWIDTH_EXTRA_CONDENSED:ঘূৰাই 62.5f;
    পক্ষ FWIDTH_CONDENSED:	ঘূৰাই 75.f;
    পক্ষ FWIDTH_SEMI_CONDENSED:	ঘূৰাই 87.5f;
    স্ৱভাৱ:
    পক্ষ FWIDTH_NORMAL:		ঘূৰাই 100.f;
    পক্ষ FWIDTH_SEMI_EXPANDED:	ঘূৰাই 112.5f;
    পক্ষ FWIDTH_EXPANDED:	ঘূৰাই 125.f;
    পক্ষ FWIDTH_EXTRA_EXPANDED:	ঘূৰাই 150.f;
    পক্ষ FWIDTH_ULTRA_EXPANDED:	ঘূৰাই 200.f;
    }
  }

  ভিন্ন map_wdth_to_widthclass(ভিন্ন width) ধ্রুৱ
  {
    যদি (width < 50) ঘূৰাই 1.0f;
    যদি (width > 200) ঘূৰাই 9.0f;

    ভিন্ন ratio = (width - 50) / 12.5f;
    পূর্ণাঙ্ক a = (পূর্ণাঙ্ক) floorf (ratio);
    পূর্ণাঙ্ক b = (পূর্ণাঙ্ক) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    যদি (b <= 6) // 50-125
    {
      যদি (a == b) ঘূৰাই a + 1.0f;
    }
    অন্যথা যদি (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    অন্যথা যদি (b == 8)
    {
      যদি (a == b) ঘূৰাই 8.0f; // 150
      a = 6;
    }
    অন্যথা
    {
      যদি (a == b && a == 12) ঘূৰাই 9.0f; //200
      b = 12;
      a = 8;
    }

    ভিন্ন va = 50 + a * 12.5f;
    ভিন্ন vb = 50 + b * 12.5f;

    ভিন্ন ret =  a + (width - va) / (vb - va);
    যদি (a <= 6) ret += 1.0f;
    ঘূৰাই ret;
  }

  স্থির unsigned calc_avg_char_width (ধ্রুৱ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, পূর্ণাঙ্ক>>& hmtx_map)
  {
    unsigned অঙ্ক = 0;
    unsigned total_width = 0;
    বাবে (ধ্রুৱ auto& _ : hmtx_map.মানসমূহ_ref ())
    {
      unsigned width = _.first;
      যদি (width)
      {
        total_width += width;
        অঙ্ক++;
      }
    }

    ঘূৰাই অঙ্ক ? (unsigned) roundf ((দ্ৱিগুণ) total_width / (দ্ৱিগুণ) অঙ্ক) : 0;
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    OS2 *os2_মৌলিক = c->serializer->embed (এতত্);
    যদি (unlikely (!os2_মৌলিক)) return_trace (মিছা);

#ifndef HB_NO_VAR
    যদি (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_মৌলিক;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP,         s_Typo_সাৰি_Gap);
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

      যদি (os2_মৌলিক->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2V2Tail &> (os2_মৌলিক->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      যদি (!c->serializer->check_assign (os2_মৌলিক->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (মিছা);
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

    os2_মৌলিক->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_মৌলিক->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (সঁচা);

    _update_unicode_ranges (&c->plan->unicodes, os2_মৌলিক->ulUnicodeRange);

    return_trace (সঁচা);
  }

  শূন্য _update_unicode_ranges (ধ্রুৱ hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ধ্রুৱ
  {
    HBUINT32 newBits[4];
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    বাবে (auto cp : *codepoints)
    {
      unsigned পূর্ণাঙ্ক bit = _hb_ot_os2_get_unicode_range_bit (cp);
      যদি (bit < 128)
      {
	unsigned পূর্ণাঙ্ক block = bit / 32;
	unsigned পূর্ণাঙ্ক bit_in_block = bit % 32;
	unsigned পূর্ণাঙ্ক mask = 1 << bit_in_block;
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

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < 4; i++)
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
  font_page_t get_font_page () ধ্রুৱ
  { ঘূৰাই (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_size () ধ্রুৱ
  {
    size_t result = min_size;
    যদি (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    যদি (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    যদি (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    ঘূৰাই result;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্))) return_trace (মিছা);
    hb_barrier ();
    যদি (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (মিছা);
    যদি (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (মিছা);
    যদি (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (মিছা);
    return_trace (সঁচা);
  }

  সার্ৱজনিক:
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
  HBINT16	s_Typo_সাৰি_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2V1Tail	v1X;
  OS2V2Tail	v2X;
  OS2V5Tail	v5X;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
