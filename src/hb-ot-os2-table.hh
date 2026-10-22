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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-os2-unicode-ranges.hh"
#अन्तर्भाव "hb-ot-var-mvar-table.hh"

#अन्तर्भाव "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


नामाकाश OT {

संरचना OS2V1Tail
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना OS2V2Tail
{
  तार्किक has_data () ध्रुव { प्रत्यागम sxHeight || sCapHeight; }

  ध्रुव OS2V2Tail * operator -> () ध्रुव { प्रत्यागम एतत्; }
  OS2V2Tail * operator -> () { प्रत्यागम एतत्; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (10);
};

संरचना OS2V5Tail
{
  अन्तःस्थ तार्किक get_optical_size (unsigned पूर्णाङ्क *lower, unsigned पूर्णाङ्क *upper) ध्रुव
  {
    unsigned पूर्णाङ्क lower_optical_size = usLowerOpticalPointSize;
    unsigned पूर्णाङ्क upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    जइ (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      प्रत्यागम सच्च;
    }
    प्रत्यागम मिच्छ;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT16	usLowerOpticalPointSize;
  HBUINT16	usUpperOpticalPointSize;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना OS2
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  तार्किक has_data () ध्रुव { प्रत्यागम usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  ध्रुव OS2V1Tail &v1 () ध्रुव { प्रत्यागम version >= 1 ? v1X : Null (OS2V1Tail); }
  ध्रुव OS2V2Tail &v2 () ध्रुव { प्रत्यागम version >= 2 ? v2X : Null (OS2V2Tail); }
  ध्रुव OS2V5Tail &v5 () ध्रुव { प्रत्यागम version >= 5 ? v5X : Null (OS2V5Tail); }

  गणना selection_flag_t {
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

  तार्किक        is_italic () ध्रुव { प्रत्यागम fsSelection & ITALIC; }
  तार्किक       is_oblique () ध्रुव { प्रत्यागम fsSelection & OBLIQUE; }
  तार्किक use_typo_metrics () ध्रुव { प्रत्यागम fsSelection & USE_TYPO_METRICS; }

  गणना width_class_t {
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

  भिन्न get_width () ध्रुव
  {
    विकल्प (usWidthClass) {
    पक्ष FWIDTH_ULTRA_CONDENSED:प्रत्यागम 50.f;
    पक्ष FWIDTH_EXTRA_CONDENSED:प्रत्यागम 62.5f;
    पक्ष FWIDTH_CONDENSED:	प्रत्यागम 75.f;
    पक्ष FWIDTH_SEMI_CONDENSED:	प्रत्यागम 87.5f;
    स्वभाव:
    पक्ष FWIDTH_NORMAL:		प्रत्यागम 100.f;
    पक्ष FWIDTH_SEMI_EXPANDED:	प्रत्यागम 112.5f;
    पक्ष FWIDTH_EXPANDED:	प्रत्यागम 125.f;
    पक्ष FWIDTH_EXTRA_EXPANDED:	प्रत्यागम 150.f;
    पक्ष FWIDTH_ULTRA_EXPANDED:	प्रत्यागम 200.f;
    }
  }

  भिन्न map_wdth_to_widthclass(भिन्न width) ध्रुव
  {
    जइ (width < 50) प्रत्यागम 1.0f;
    जइ (width > 200) प्रत्यागम 9.0f;

    भिन्न ratio = (width - 50) / 12.5f;
    पूर्णाङ्क a = (पूर्णाङ्क) floorf (ratio);
    पूर्णाङ्क b = (पूर्णाङ्क) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    जइ (b <= 6) // 50-125
    {
      जइ (a == b) प्रत्यागम a + 1.0f;
    }
    अण्णहा जइ (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    अण्णहा जइ (b == 8)
    {
      जइ (a == b) प्रत्यागम 8.0f; // 150
      a = 6;
    }
    अण्णहा
    {
      जइ (a == b && a == 12) प्रत्यागम 9.0f; //200
      b = 12;
      a = 8;
    }

    भिन्न va = 50 + a * 12.5f;
    भिन्न vb = 50 + b * 12.5f;

    भिन्न ret =  a + (width - va) / (vb - va);
    जइ (a <= 6) ret += 1.0f;
    प्रत्यागम ret;
  }

  स्थिर unsigned calc_avg_char_width (ध्रुव hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, पूर्णाङ्क>>& hmtx_map)
  {
    unsigned अङ्कः = 0;
    unsigned total_width = 0;
    प्रति (ध्रुव auto& _ : hmtx_map.मानानि_ref ())
    {
      unsigned width = _.first;
      जइ (width)
      {
        total_width += width;
        अङ्कः++;
      }
    }

    प्रत्यागम अङ्कः ? (unsigned) roundf ((द्विगुण) total_width / (द्विगुण) अङ्कः) : 0;
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    OS2 *os2_अविभाज्यः = c->serializer->embed (एतत्);
    जइ (unlikely (!os2_अविभाज्यः)) return_trace (मिच्छ);

#ifndef HB_NO_VAR
    जइ (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_अविभाज्यः;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_पङ्क्तिः_GAP,         s_Typo_पङ्क्तिः_Gap);
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

      जइ (os2_अविभाज्यः->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2V2Tail &> (os2_अविभाज्यः->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      जइ (!c->serializer->check_assign (os2_अविभाज्यः->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (मिच्छ);
    }
#endif

    Triple *axis_range;
    जइ (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      unsigned weight_class = static_cast<unsigned> (roundf (hb_clamp (axis_range->मध्यः, 1.0f, 1000.0f)));
      जइ (os2_अविभाज्यः->usWeightClass != weight_class)
        os2_अविभाज्यः->usWeightClass = weight_class;
    }

    जइ (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      unsigned width_class = static_cast<unsigned> (roundf (map_wdth_to_widthclass (axis_range->मध्यः)));
      जइ (os2_अविभाज्यः->usWidthClass != width_class)
        os2_अविभाज्यः->usWidthClass = width_class;
    }

    os2_अविभाज्यः->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_अविभाज्यः->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    जइ (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (सच्च);

    _update_unicode_ranges (&c->plan->unicodes, os2_अविभाज्यः->ulUnicodeRange);

    return_trace (सच्च);
  }

  शून्य _update_unicode_ranges (ध्रुव hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ध्रुव
  {
    HBUINT32 newBits[4];
    प्रति (unsigned पूर्णाङ्क i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    प्रति (auto cp : *codepoints)
    {
      unsigned पूर्णाङ्क bit = _hb_ot_os2_get_unicode_range_bit (cp);
      जइ (bit < 128)
      {
	unsigned पूर्णाङ्क block = bit / 32;
	unsigned पूर्णाङ्क bit_in_block = bit % 32;
	unsigned पूर्णाङ्क mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      जइ (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    प्रति (unsigned पूर्णाङ्क i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  गणना font_page_t
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
  font_page_t get_font_page () ध्रुव
  { प्रत्यागम (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_size () ध्रुव
  {
    size_t result = min_size;
    जइ (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    जइ (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    जइ (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    प्रत्यागम result;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (unlikely (!c->check_struct (एतत्))) return_trace (मिच्छ);
    hb_barrier ();
    जइ (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (मिच्छ);
    जइ (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (मिच्छ);
    जइ (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (मिच्छ);
    return_trace (सच्च);
  }

  सार्वजनिक:
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
  HBINT16	s_Typo_पङ्क्तिः_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2V1Tail	v1X;
  OS2V2Tail	v2X;
  OS2V5Tail	v5X;
  सार्वजनिक:
  DEFINE_SIZE_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
