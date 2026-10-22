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
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  सार्वजनिक:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना OS2V2Tail
{
  तार्किक has_data () स्थिरांक { परत sxHeight || sCapHeight; }

  स्थिरांक OS2V2Tail * चालक -> () स्थिरांक { परत हे; }
  OS2V2Tail * चालक -> () { परत हे; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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
  ओळीत तार्किक get_optical_size (अचिन्हित पूर्णांक *lower, अचिन्हित पूर्णांक *upper) स्थिरांक
  {
    अचिन्हित पूर्णांक lower_optical_size = usLowerOpticalPointSize;
    अचिन्हित पूर्णांक upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    जर (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      परत सत्य;
    }
    परत असत्य;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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

  तार्किक has_data () स्थिरांक { परत usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  स्थिरांक OS2V1Tail &v1 () स्थिरांक { परत version >= 1 ? v1X : Null (OS2V1Tail); }
  स्थिरांक OS2V2Tail &v2 () स्थिरांक { परत version >= 2 ? v2X : Null (OS2V2Tail); }
  स्थिरांक OS2V5Tail &v5 () स्थिरांक { परत version >= 5 ? v5X : Null (OS2V5Tail); }

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

  तार्किक        is_italic () स्थिरांक { परत fsSelection & ITALIC; }
  तार्किक       is_oblique () स्थिरांक { परत fsSelection & OBLIQUE; }
  तार्किक use_typo_metrics () स्थिरांक { परत fsSelection & USE_TYPO_METRICS; }

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

  दशांश get_width () स्थिरांक
  {
    निवड (usWidthClass) {
    स्थिती FWIDTH_ULTRA_CONDENSED:परत 50.f;
    स्थिती FWIDTH_EXTRA_CONDENSED:परत 62.5f;
    स्थिती FWIDTH_CONDENSED:	परत 75.f;
    स्थिती FWIDTH_SEMI_CONDENSED:	परत 87.5f;
    पूर्वनिर्धारित:
    स्थिती FWIDTH_NORMAL:		परत 100.f;
    स्थिती FWIDTH_SEMI_EXPANDED:	परत 112.5f;
    स्थिती FWIDTH_EXPANDED:	परत 125.f;
    स्थिती FWIDTH_EXTRA_EXPANDED:	परत 150.f;
    स्थिती FWIDTH_ULTRA_EXPANDED:	परत 200.f;
    }
  }

  दशांश map_wdth_to_widthclass(दशांश width) स्थिरांक
  {
    जर (width < 50) परत 1.0f;
    जर (width > 200) परत 9.0f;

    दशांश ratio = (width - 50) / 12.5f;
    पूर्णांक a = (पूर्णांक) floorf (ratio);
    पूर्णांक b = (पूर्णांक) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    जर (b <= 6) // 50-125
    {
      जर (a == b) परत a + 1.0f;
    }
    अन्यथा जर (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    अन्यथा जर (b == 8)
    {
      जर (a == b) परत 8.0f; // 150
      a = 6;
    }
    अन्यथा
    {
      जर (a == b && a == 12) परत 9.0f; //200
      b = 12;
      a = 8;
    }

    दशांश va = 50 + a * 12.5f;
    दशांश vb = 50 + b * 12.5f;

    दशांश ret =  a + (width - va) / (vb - va);
    जर (a <= 6) ret += 1.0f;
    परत ret;
  }

  स्थिर अचिन्हित calc_avg_char_width (स्थिरांक hb_hashmap_t<hb_codepoint_t, hb_pair_t<अचिन्हित, पूर्णांक>>& hmtx_map)
  {
    अचिन्हित अंक = 0;
    अचिन्हित total_width = 0;
    साठी (स्थिरांक स्वतः& _ : hmtx_map.मूल्ये_ref ())
    {
      अचिन्हित width = _.first;
      जर (width)
      {
        total_width += width;
        अंक++;
      }
    }

    परत अंक ? (अचिन्हित) roundf ((द्विगुण) total_width / (द्विगुण) अंक) : 0;
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    OS2 *os2_मूळसंख्या = c->serializer->embed (हे);
    जर (unlikely (!os2_मूळसंख्या)) return_trace (असत्य);

#ifndef HB_NO_VAR
    जर (c->plan->normalized_coords)
    {
      स्वतः &MVAR = *c->plan->source->table.MVAR;
      स्वतः *table = os2_मूळसंख्या;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ओळ_GAP,         s_Typo_ओळ_Gap);
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

      जर (os2_मूळसंख्या->version >= 2)
      {
        hb_barrier ();
        स्वतः *table = & const_cast<OS2V2Tail &> (os2_मूळसंख्या->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      अचिन्हित avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      जर (!c->serializer->check_assign (os2_मूळसंख्या->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (असत्य);
    }
#endif

    Triple *axis_range;
    जर (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      अचिन्हित weight_class = static_cast<अचिन्हित> (roundf (hb_clamp (axis_range->मधला, 1.0f, 1000.0f)));
      जर (os2_मूळसंख्या->usWeightClass != weight_class)
        os2_मूळसंख्या->usWeightClass = weight_class;
    }

    जर (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      अचिन्हित width_class = static_cast<अचिन्हित> (roundf (map_wdth_to_widthclass (axis_range->मधला)));
      जर (os2_मूळसंख्या->usWidthClass != width_class)
        os2_मूळसंख्या->usWidthClass = width_class;
    }

    os2_मूळसंख्या->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_मूळसंख्या->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    जर (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (सत्य);

    _update_unicode_ranges (&c->plan->unicodes, os2_मूळसंख्या->ulUnicodeRange);

    return_trace (सत्य);
  }

  शून्य _update_unicode_ranges (स्थिरांक hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) स्थिरांक
  {
    HBUINT32 newBits[4];
    साठी (अचिन्हित पूर्णांक i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    साठी (स्वतः cp : *codepoints)
    {
      अचिन्हित पूर्णांक bit = _hb_ot_os2_get_unicode_range_bit (cp);
      जर (bit < 128)
      {
	अचिन्हित पूर्णांक block = bit / 32;
	अचिन्हित पूर्णांक bit_in_block = bit % 32;
	अचिन्हित पूर्णांक mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      जर (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    साठी (अचिन्हित पूर्णांक i = 0; i < 4; i++)
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
  font_page_t get_font_page () स्थिरांक
  { परत (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  माप_प्रकार get_size () स्थिरांक
  {
    माप_प्रकार result = min_size;
    जर (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    जर (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    जर (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    परत result;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे))) return_trace (असत्य);
    hb_barrier ();
    जर (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (असत्य);
    जर (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (असत्य);
    जर (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (असत्य);
    return_trace (सत्य);
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
  HBINT16	s_Typo_ओळ_Gap;
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
