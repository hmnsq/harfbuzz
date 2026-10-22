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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-os2-unicode-ranges.hh"
#അന്തര്ഭാവ "hb-ot-var-mvar-table.hh"

#അന്തര്ഭാവ "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


നാമാകാശ OT {

ഘടന OS2V1Tail
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന OS2V2Tail
{
  തർക്കം has_data () സ്ഥിരം { തിരികെ sxHeight || sCapHeight; }

  സ്ഥിരം OS2V2Tail * operator -> () സ്ഥിരം { തിരികെ ഇത്; }
  OS2V2Tail * operator -> () { തിരികെ ഇത്; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  പൊതു:
  DEFINE_SIZE_STATIC (10);
};

ഘടന OS2V5Tail
{
  വരിയിൽ തർക്കം get_optical_size (unsigned പൂർണ്ണസംഖ്യ *lower, unsigned പൂർണ്ണസംഖ്യ *upper) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ lower_optical_size = usLowerOpticalPointSize;
    unsigned പൂർണ്ണസംഖ്യ upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    എങ്കിൽ (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT16	usLowerOpticalPointSize;
  HBUINT16	usUpperOpticalPointSize;
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന OS2
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  തർക്കം has_data () സ്ഥിരം { തിരികെ usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  സ്ഥിരം OS2V1Tail &v1 () സ്ഥിരം { തിരികെ version >= 1 ? v1X : Null (OS2V1Tail); }
  സ്ഥിരം OS2V2Tail &v2 () സ്ഥിരം { തിരികെ version >= 2 ? v2X : Null (OS2V2Tail); }
  സ്ഥിരം OS2V5Tail &v5 () സ്ഥിരം { തിരികെ version >= 5 ? v5X : Null (OS2V5Tail); }

  എണ്ണൽ selection_flag_t {
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

  തർക്കം        is_italic () സ്ഥിരം { തിരികെ fsSelection & ITALIC; }
  തർക്കം       is_oblique () സ്ഥിരം { തിരികെ fsSelection & OBLIQUE; }
  തർക്കം use_typo_metrics () സ്ഥിരം { തിരികെ fsSelection & USE_TYPO_METRICS; }

  എണ്ണൽ width_class_t {
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

  ദശാംശം get_width () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (usWidthClass) {
    അവസ്ഥ FWIDTH_ULTRA_CONDENSED:തിരികെ 50.f;
    അവസ്ഥ FWIDTH_EXTRA_CONDENSED:തിരികെ 62.5f;
    അവസ്ഥ FWIDTH_CONDENSED:	തിരികെ 75.f;
    അവസ്ഥ FWIDTH_SEMI_CONDENSED:	തിരികെ 87.5f;
    സ്ഥിരസ്ഥിതി:
    അവസ്ഥ FWIDTH_NORMAL:		തിരികെ 100.f;
    അവസ്ഥ FWIDTH_SEMI_EXPANDED:	തിരികെ 112.5f;
    അവസ്ഥ FWIDTH_EXPANDED:	തിരികെ 125.f;
    അവസ്ഥ FWIDTH_EXTRA_EXPANDED:	തിരികെ 150.f;
    അവസ്ഥ FWIDTH_ULTRA_EXPANDED:	തിരികെ 200.f;
    }
  }

  ദശാംശം map_wdth_to_widthclass(ദശാംശം width) സ്ഥിരം
  {
    എങ്കിൽ (width < 50) തിരികെ 1.0f;
    എങ്കിൽ (width > 200) തിരികെ 9.0f;

    ദശാംശം ratio = (width - 50) / 12.5f;
    പൂർണ്ണസംഖ്യ a = (പൂർണ്ണസംഖ്യ) floorf (ratio);
    പൂർണ്ണസംഖ്യ b = (പൂർണ്ണസംഖ്യ) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    എങ്കിൽ (b <= 6) // 50-125
    {
      എങ്കിൽ (a == b) തിരികെ a + 1.0f;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (b == 8)
    {
      എങ്കിൽ (a == b) തിരികെ 8.0f; // 150
      a = 6;
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (a == b && a == 12) തിരികെ 9.0f; //200
      b = 12;
      a = 8;
    }

    ദശാംശം va = 50 + a * 12.5f;
    ദശാംശം vb = 50 + b * 12.5f;

    ദശാംശം ret =  a + (width - va) / (vb - va);
    എങ്കിൽ (a <= 6) ret += 1.0f;
    തിരികെ ret;
  }

  സ്ഥിതം unsigned calc_avg_char_width (സ്ഥിരം hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>>& hmtx_map)
  {
    unsigned അക്കം = 0;
    unsigned total_width = 0;
    വേണ്ടി (സ്ഥിരം auto& _ : hmtx_map.മൂല്യങ്ങൾ_ref ())
    {
      unsigned width = _.first;
      എങ്കിൽ (width)
      {
        total_width += width;
        അക്കം++;
      }
    }

    തിരികെ അക്കം ? (unsigned) roundf ((ഇരട്ട) total_width / (ഇരട്ട) അക്കം) : 0;
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    OS2 *os2_അഭാജ്യം = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!os2_അഭാജ്യം)) return_trace (അസത്യം);

#ifndef HB_NO_VAR
    എങ്കിൽ (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_അഭാജ്യം;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP,         s_Typo_വരി_Gap);
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

      എങ്കിൽ (os2_അഭാജ്യം->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2V2Tail &> (os2_അഭാജ്യം->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      എങ്കിൽ (!c->serializer->check_assign (os2_അഭാജ്യം->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (അസത്യം);
    }
#endif

    Triple *axis_range;
    എങ്കിൽ (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      unsigned weight_class = static_cast<unsigned> (roundf (hb_clamp (axis_range->നടുവ്, 1.0f, 1000.0f)));
      എങ്കിൽ (os2_അഭാജ്യം->usWeightClass != weight_class)
        os2_അഭാജ്യം->usWeightClass = weight_class;
    }

    എങ്കിൽ (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      unsigned width_class = static_cast<unsigned> (roundf (map_wdth_to_widthclass (axis_range->നടുവ്)));
      എങ്കിൽ (os2_അഭാജ്യം->usWidthClass != width_class)
        os2_അഭാജ്യം->usWidthClass = width_class;
    }

    os2_അഭാജ്യം->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_അഭാജ്യം->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    എങ്കിൽ (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (സത്യം);

    _update_unicode_ranges (&c->plan->unicodes, os2_അഭാജ്യം->ulUnicodeRange);

    return_trace (സത്യം);
  }

  ശൂന്യം _update_unicode_ranges (സ്ഥിരം hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) സ്ഥിരം
  {
    HBUINT32 newBits[4];
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    വേണ്ടി (auto cp : *codepoints)
    {
      unsigned പൂർണ്ണസംഖ്യ bit = _hb_ot_os2_get_unicode_range_bit (cp);
      എങ്കിൽ (bit < 128)
      {
	unsigned പൂർണ്ണസംഖ്യ block = bit / 32;
	unsigned പൂർണ്ണസംഖ്യ bit_in_block = bit % 32;
	unsigned പൂർണ്ണസംഖ്യ mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      എങ്കിൽ (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  എണ്ണൽ font_page_t
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
  font_page_t get_font_page () സ്ഥിരം
  { തിരികെ (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_size () സ്ഥിരം
  {
    size_t result = min_size;
    എങ്കിൽ (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    എങ്കിൽ (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    എങ്കിൽ (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    തിരികെ result;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്))) return_trace (അസത്യം);
    hb_barrier ();
    എങ്കിൽ (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  പൊതു:
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
  HBINT16	s_Typo_വരി_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2V1Tail	v1X;
  OS2V2Tail	v2X;
  OS2V5Tail	v5X;
  പൊതു:
  DEFINE_SIZE_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
