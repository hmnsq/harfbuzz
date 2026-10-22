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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-os2-unicode-ranges.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-mvar-table.hh"

#ଅନ୍ତର୍ଭାଵ "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


ନାମାକାଶ OT {

ସଂରଚନା OS2V1Tail
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା OS2V2Tail
{
  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ sxHeight || sCapHeight; }

  ଅଚଳ OS2V2Tail * operator -> () ଅଚଳ { ଫେରନ୍ତୁ ଏହି; }
  OS2V2Tail * operator -> () { ଫେରନ୍ତୁ ଏହି; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (10);
};

ସଂରଚନା OS2V5Tail
{
  ଧାଡ଼ିଭିତରେ ତାର୍କିକ get_optical_size (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lower, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *upper) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lower_optical_size = usLowerOpticalPointSize;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    ଯଦି (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBUINT16	usLowerOpticalPointSize;
  HBUINT16	usUpperOpticalPointSize;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା OS2
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  ଅଚଳ OS2V1Tail &v1 () ଅଚଳ { ଫେରନ୍ତୁ version >= 1 ? v1X : Null (OS2V1Tail); }
  ଅଚଳ OS2V2Tail &v2 () ଅଚଳ { ଫେରନ୍ତୁ version >= 2 ? v2X : Null (OS2V2Tail); }
  ଅଚଳ OS2V5Tail &v5 () ଅଚଳ { ଫେରନ୍ତୁ version >= 5 ? v5X : Null (OS2V5Tail); }

  ଗଣନା selection_flag_t {
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

  ତାର୍କିକ        is_italic () ଅଚଳ { ଫେରନ୍ତୁ fsSelection & ITALIC; }
  ତାର୍କିକ       is_oblique () ଅଚଳ { ଫେରନ୍ତୁ fsSelection & OBLIQUE; }
  ତାର୍କିକ use_typo_metrics () ଅଚଳ { ଫେରନ୍ତୁ fsSelection & USE_TYPO_METRICS; }

  ଗଣନା width_class_t {
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

  ଦଶମିକ get_width () ଅଚଳ
  {
    ବାଛ (usWidthClass) {
    ସ୍ଥିତି FWIDTH_ULTRA_CONDENSED:ଫେରନ୍ତୁ 50.f;
    ସ୍ଥିତି FWIDTH_EXTRA_CONDENSED:ଫେରନ୍ତୁ 62.5f;
    ସ୍ଥିତି FWIDTH_CONDENSED:	ଫେରନ୍ତୁ 75.f;
    ସ୍ଥିତି FWIDTH_SEMI_CONDENSED:	ଫେରନ୍ତୁ 87.5f;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    ସ୍ଥିତି FWIDTH_NORMAL:		ଫେରନ୍ତୁ 100.f;
    ସ୍ଥିତି FWIDTH_SEMI_EXPANDED:	ଫେରନ୍ତୁ 112.5f;
    ସ୍ଥିତି FWIDTH_EXPANDED:	ଫେରନ୍ତୁ 125.f;
    ସ୍ଥିତି FWIDTH_EXTRA_EXPANDED:	ଫେରନ୍ତୁ 150.f;
    ସ୍ଥିତି FWIDTH_ULTRA_EXPANDED:	ଫେରନ୍ତୁ 200.f;
    }
  }

  ଦଶମିକ map_wdth_to_widthclass(ଦଶମିକ width) ଅଚଳ
  {
    ଯଦି (width < 50) ଫେରନ୍ତୁ 1.0f;
    ଯଦି (width > 200) ଫେରନ୍ତୁ 9.0f;

    ଦଶମିକ ratio = (width - 50) / 12.5f;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) floorf (ratio);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    ଯଦି (b <= 6) // 50-125
    {
      ଯଦି (a == b) ଫେରନ୍ତୁ a + 1.0f;
    }
    ଅନ୍ୟଥା ଯଦି (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    ଅନ୍ୟଥା ଯଦି (b == 8)
    {
      ଯଦି (a == b) ଫେରନ୍ତୁ 8.0f; // 150
      a = 6;
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (a == b && a == 12) ଫେରନ୍ତୁ 9.0f; //200
      b = 12;
      a = 8;
    }

    ଦଶମିକ va = 50 + a * 12.5f;
    ଦଶମିକ vb = 50 + b * 12.5f;

    ଦଶମିକ ret =  a + (width - va) / (vb - va);
    ଯଦି (a <= 6) ret += 1.0f;
    ଫେରନ୍ତୁ ret;
  }

  ସ୍ଥିର unsigned calc_avg_char_width (ଅଚଳ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>& hmtx_map)
  {
    unsigned ଅଙ୍କ = 0;
    unsigned total_width = 0;
    ପାଇଁ (ଅଚଳ auto& _ : hmtx_map.ମୂଲ୍ୟଗୁଡ଼ିକ_ref ())
    {
      unsigned width = _.first;
      ଯଦି (width)
      {
        total_width += width;
        ଅଙ୍କ++;
      }
    }

    ଫେରନ୍ତୁ ଅଙ୍କ ? (unsigned) roundf ((ଦ୍ୱିଗୁଣ) total_width / (ଦ୍ୱିଗୁଣ) ଅଙ୍କ) : 0;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    OS2 *os2_ମୌଳିକ = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!os2_ମୌଳିକ)) return_trace (ମିଥ୍ୟା);

#ifndef HB_NO_VAR
    ଯଦି (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_ମୌଳିକ;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP,         s_Typo_ଧାଡ଼ି_Gap);
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

      ଯଦି (os2_ମୌଳିକ->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2V2Tail &> (os2_ମୌଳିକ->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      ଯଦି (!c->serializer->check_assign (os2_ମୌଳିକ->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (ମିଥ୍ୟା);
    }
#endif

    Triple *axis_range;
    ଯଦି (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      unsigned weight_class = static_cast<unsigned> (roundf (hb_clamp (axis_range->ମଧ୍ୟ, 1.0f, 1000.0f)));
      ଯଦି (os2_ମୌଳିକ->usWeightClass != weight_class)
        os2_ମୌଳିକ->usWeightClass = weight_class;
    }

    ଯଦି (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      unsigned width_class = static_cast<unsigned> (roundf (map_wdth_to_widthclass (axis_range->ମଧ୍ୟ)));
      ଯଦି (os2_ମୌଳିକ->usWidthClass != width_class)
        os2_ମୌଳିକ->usWidthClass = width_class;
    }

    os2_ମୌଳିକ->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_ମୌଳିକ->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    ଯଦି (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (ସତ୍ୟ);

    _update_unicode_ranges (&c->plan->unicodes, os2_ମୌଳିକ->ulUnicodeRange);

    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ _update_unicode_ranges (ଅଚଳ hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ଅଚଳ
  {
    HBUINT32 newBits[4];
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    ପାଇଁ (auto cp : *codepoints)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit = _hb_ot_os2_get_unicode_range_bit (cp);
      ଯଦି (bit < 128)
      {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା block = bit / 32;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_in_block = bit % 32;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      ଯଦି (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  ଗଣନା font_page_t
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
  font_page_t get_font_page () ଅଚଳ
  { ଫେରନ୍ତୁ (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_size () ଅଚଳ
  {
    size_t result = min_size;
    ଯଦି (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    ଯଦି (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    ଯଦି (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    ଫେରନ୍ତୁ result;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଯଦି (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ସର୍ବସାଧାରଣ:
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
  HBINT16	s_Typo_ଧାଡ଼ି_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2V1Tail	v1X;
  OS2V2Tail	v2X;
  OS2V5Tail	v5X;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
