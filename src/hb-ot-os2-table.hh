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

#अगर_अघोषित HB_OT_OS2_TABLE_HH
#घोषणा HB_OT_OS2_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-os2-unicode-ranges.hh"
#समावेश "hb-ot-var-mvar-table.hh"

#समावेश "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#घोषणा HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


namespace OT {

काष्ठा OS2_V1_पुच्छ
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा OS2_V2_पुच्छ
{
  bool has_तथ्य () स्थिर { वापस sxHeight || sCapHeight; }

  स्थिर OS2_V2_पुच्छ * चालक -> () स्थिर { वापस यह; }
  OS2_V2_पुच्छ * चालक -> () { वापस यह; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  खुला:
  DEFINE_आकार_STATIC (10);
};

काष्ठा OS2_V5_पुच्छ
{
  अंतरभूत bool get_optical_आकार (अचिन्हित पूर्णांक *lower, अचिन्हित पूर्णांक *upper) स्थिर
  {
    अचिन्हित पूर्णांक lower_optical_आकार = us_Lower_Optical_Point_आकार;
    अचिन्हित पूर्णांक upper_optical_आकार = us_Upper_Optical_Point_आकार;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    अगर (lower_optical_आकार < upper_optical_आकार &&
	lower_optical_आकार >= 1 && lower_optical_आकार <= 0xFFFE &&
	upper_optical_आकार >= 2 && upper_optical_आकार <= 0xFFFF)
    {
      *lower = lower_optical_आकार;
      *upper = upper_optical_आकार;
      वापस true;
    }
    वापस false;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT16	us_Lower_Optical_Point_आकार;
  HBUINT16	us_Upper_Optical_Point_आकार;
  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा OS2
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  bool has_तथ्य () स्थिर { वापस usWeightClass || usWidthClass || us_First_Char_सूचकांक || us_Last_Char_सूचकांक; }

  स्थिर OS2_V1_पुच्छ &v1 () स्थिर { वापस version >= 1 ? v1X : Null (OS2_V1_पुच्छ); }
  स्थिर OS2_V2_पुच्छ &v2 () स्थिर { वापस version >= 2 ? v2X : Null (OS2_V2_पुच्छ); }
  स्थिर OS2_V5_पुच्छ &v5 () स्थिर { वापस version >= 5 ? v5X : Null (OS2_V5_पुच्छ); }

  क्रमागत selection_flag_t {
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

  bool        is_italic () स्थिर { वापस fsSelection & ITALIC; }
  bool       is_oblique () स्थिर { वापस fsSelection & OBLIQUE; }
  bool use_typo_metrics () स्थिर { वापस fsSelection & USE_TYPO_METRICS; }

  क्रमागत width_class_t {
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

  भग्न get_width () स्थिर
  {
    चयन (usWidthClass) {
    हाल FWIDTH_ULTRA_CONDENSED:वापस 50.f;
    हाल FWIDTH_EXTRA_CONDENSED:वापस 62.5f;
    हाल FWIDTH_CONDENSED:	वापस 75.f;
    हाल FWIDTH_SEMI_CONDENSED:	वापस 87.5f;
    शेष:
    हाल FWIDTH_NORMAL:		वापस 100.f;
    हाल FWIDTH_SEMI_EXPANDED:	वापस 112.5f;
    हाल FWIDTH_EXPANDED:	वापस 125.f;
    हाल FWIDTH_EXTRA_EXPANDED:	वापस 150.f;
    हाल FWIDTH_ULTRA_EXPANDED:	वापस 200.f;
    }
  }

  भग्न map_wdth_to_widthclass(भग्न width) स्थिर
  {
    अगर (width < 50) वापस 1.0f;
    अगर (width > 200) वापस 9.0f;

    भग्न ratio = (width - 50) / 12.5f;
    पूर्णांक a = (पूर्णांक) floorf (ratio);
    पूर्णांक b = (पूर्णांक) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    अगर (b <= 6) // 50-125
    {
      अगर (a == b) वापस a + 1.0f;
    }
    अन्यथा अगर (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    अन्यथा अगर (b == 8)
    {
      अगर (a == b) वापस 8.0f; // 150
      a = 6;
    }
    अन्यथा
    {
      अगर (a == b && a == 12) वापस 9.0f; //200
      b = 12;
      a = 8;
    }

    भग्न va = 50 + a * 12.5f;
    भग्न vb = 50 + b * 12.5f;

    भग्न ret =  a + (width - va) / (vb - va);
    अगर (a <= 6) ret += 1.0f;
    वापस ret;
  }

  अटल अचिन्हित calc_avg_char_width (स्थिर hb_hashmap_t<hb_codepoint_t, hb_pair_t<अचिन्हित, पूर्णांक>>& hmtx_map)
  {
    अचिन्हित अंक = 0;
    अचिन्हित योग_width = 0;
    क्रम (स्थिर स्वतः& _ : hmtx_map.मूल्यावली_ref ())
    {
      अचिन्हित width = _.first;
      अगर (width)
      {
        योग_width += width;
        अंक++;
      }
    }

    वापस अंक ? (अचिन्हित) roundf ((द्विगुन) योग_width / (द्विगुन) अंक) : 0;
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    OS2 *os2_अभाज्य = c->serializer->embed (यह);
    अगर (unlikely (!os2_अभाज्य)) return_trace (false);

#अगर_अघोषित HB_NO_VAR
    अगर (c->plan->normalized_coords)
    {
      स्वतः &MVAR = *c->plan->source->table.MVAR;
      स्वतः *table = os2_अभाज्य;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP,         s_Typo_पंक्ति_Gap);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_आकार,         ySubscriptXSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_आकार,         ySubscriptYSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET,       ySubscriptXOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET,       ySubscriptYOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_आकार,       ySuperscriptXSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_आकार,       ySuperscriptYSize);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET,     ySuperscriptXOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET,     ySuperscriptYOffset);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_STRIKEOUT_आकार,              y_Strikeout_आकार);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_STRIKEOUT_OFFSET,            yStrikeoutPosition);

      अगर (os2_अभाज्य->version >= 2)
      {
        hb_barrier ();
        स्वतः *table = & const_cast<OS2_V2_पुच्छ &> (os2_अभाज्य->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      अचिन्हित avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      अगर (!c->serializer->check_assign (os2_अभाज्य->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (false);
    }
#पूर्ण_अगर

    Triple *axis_range;
    अगर (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      अचिन्हित weight_class = static_cast<अचिन्हित> (roundf (hb_clamp (axis_range->मध्य, 1.0f, 1000.0f)));
      अगर (os2_अभाज्य->usWeightClass != weight_class)
        os2_अभाज्य->usWeightClass = weight_class;
    }

    अगर (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      अचिन्हित width_class = static_cast<अचिन्हित> (roundf (map_wdth_to_widthclass (axis_range->मध्य)));
      अगर (os2_अभाज्य->usWidthClass != width_class)
        os2_अभाज्य->usWidthClass = width_class;
    }

    os2_अभाज्य->us_First_Char_सूचकांक = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_अभाज्य->us_Last_Char_सूचकांक  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    अगर (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (true);

    _update_unicode_ranges (&c->plan->unicodes, os2_अभाज्य->ulUnicodeRange);

    return_trace (true);
  }

  व्योम _update_unicode_ranges (स्थिर hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) स्थिर
  {
    HBUINT32 newBits[4];
    क्रम (अचिन्हित पूर्णांक i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    क्रम (स्वतः cp : *codepoints)
    {
      अचिन्हित पूर्णांक bit = _hb_ot_os2_get_unicode_range_bit (cp);
      अगर (bit < 128)
      {
	अचिन्हित पूर्णांक block = bit / 32;
	अचिन्हित पूर्णांक bit_in_block = bit % 32;
	अचिन्हित पूर्णांक mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      अगर (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    क्रम (अचिन्हित पूर्णांक i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  क्रमागत font_page_t
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
  font_page_t get_font_page () स्थिर
  { वापस (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  माप_प्रकार get_आकार () स्थिर
  {
    माप_प्रकार परिणाम = min_आकार;
    अगर (version >= 1) परिणाम = hb_unsigned_add_saturate (परिणाम, v1X.get_आकार ());
    अगर (version >= 2) परिणाम = hb_unsigned_add_saturate (परिणाम, v2X.get_आकार ());
    अगर (version >= 5) परिणाम = hb_unsigned_add_saturate (परिणाम, v5X.get_आकार ());
    वापस परिणाम;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह))) return_trace (false);
    hb_barrier ();
    अगर (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (false);
    अगर (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (false);
    अगर (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (false);
    return_trace (true);
  }

  खुला:
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
  HBINT16	y_Strikeout_आकार;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  Tag		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_First_Char_सूचकांक;
  HBUINT16	us_Last_Char_सूचकांक;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_पंक्ति_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_पुच्छ	v1X;
  OS2_V2_पुच्छ	v2X;
  OS2_V5_पुच्छ	v5X;
  खुला:
  DEFINE_आकार_MIN (78);
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_OS2_TABLE_HH */
