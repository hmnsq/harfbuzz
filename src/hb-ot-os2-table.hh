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

#include "hb-open-type.hh"
#include "hb-ot-os2-unicode-ranges.hh"
#include "hb-ot-var-mvar-table.hh"

#include "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#define HB_OT_TAG_OS2 HB_TAG('O','S','/','2')


namespace OT {

sa_nracanaa OS2V1Tail
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  public:
  HBUINT32	ulCodePageRange1;
  HBUINT32	ulCodePageRange2;
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa OS2V2Tail
{
  bool has_data () const { ສົ່ງຄືນ sxHeight || sCapHeight; }

  const OS2V2Tail * operator -> () const { ສົ່ງຄືນ this; }
  OS2V2Tail * operator -> () { ສົ່ງຄືນ this; }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  public:
  HBINT16	sxHeight;
  HBINT16	sCapHeight;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  public:
  DEFINE_SIZE_STATIC (10);
};

sa_nracanaa OS2V5Tail
{
  anta_hstha bool get_optical_size (unsigned ຈຳນວນເຕັມ *lower, unsigned ຈຳນວນເຕັມ *upper) const
  {
    unsigned ຈຳນວນເຕັມ lower_optical_size = usLowerOpticalPointSize;
    unsigned ຈຳນວນເຕັມ upper_optical_size = usUpperOpticalPointSize;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    ຖ້າ (lower_optical_size < upper_optical_size &&
	lower_optical_size >= 1 && lower_optical_size <= 0xFFFE &&
	upper_optical_size >= 2 && upper_optical_size <= 0xFFFF)
    {
      *lower = lower_optical_size;
      *upper = upper_optical_size;
      ສົ່ງຄືນ true;
    }
    ສົ່ງຄືນ false;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  public:
  HBUINT16	usLowerOpticalPointSize;
  HBUINT16	usUpperOpticalPointSize;
  public:
  DEFINE_SIZE_STATIC (4);
};

sa_nracanaa OS2
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_OS2;

  bool has_data () const { ສົ່ງຄືນ usWeightClass || usWidthClass || usFirstCharIndex || usLastCharIndex; }

  const OS2V1Tail &v1 () const { ສົ່ງຄືນ version >= 1 ? v1X : Null (OS2V1Tail); }
  const OS2V2Tail &v2 () const { ສົ່ງຄືນ version >= 2 ? v2X : Null (OS2V2Tail); }
  const OS2V5Tail &v5 () const { ສົ່ງຄືນ version >= 5 ? v5X : Null (OS2V5Tail); }

  ga_nanaa selection_flag_t {
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

  bool        is_italic () const { ສົ່ງຄືນ fsSelection & ITALIC; }
  bool       is_oblique () const { ສົ່ງຄືນ fsSelection & OBLIQUE; }
  bool use_typo_metrics () const { ສົ່ງຄືນ fsSelection & USE_TYPO_METRICS; }

  ga_nanaa width_class_t {
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

  float get_width () const
  {
    switch (usWidthClass) {
    pak_sa FWIDTH_ULTRA_CONDENSED:ສົ່ງຄືນ 50.f;
    pak_sa FWIDTH_EXTRA_CONDENSED:ສົ່ງຄືນ 62.5f;
    pak_sa FWIDTH_CONDENSED:	ສົ່ງຄືນ 75.f;
    pak_sa FWIDTH_SEMI_CONDENSED:	ສົ່ງຄືນ 87.5f;
    default:
    pak_sa FWIDTH_NORMAL:		ສົ່ງຄືນ 100.f;
    pak_sa FWIDTH_SEMI_EXPANDED:	ສົ່ງຄືນ 112.5f;
    pak_sa FWIDTH_EXPANDED:	ສົ່ງຄືນ 125.f;
    pak_sa FWIDTH_EXTRA_EXPANDED:	ສົ່ງຄືນ 150.f;
    pak_sa FWIDTH_ULTRA_EXPANDED:	ສົ່ງຄືນ 200.f;
    }
  }

  float map_wdth_to_widthclass(float width) const
  {
    ຖ້າ (width < 50) ສົ່ງຄືນ 1.0f;
    ຖ້າ (width > 200) ສົ່ງຄືນ 9.0f;

    float ratio = (width - 50) / 12.5f;
    ຈຳນວນເຕັມ a = (ຈຳນວນເຕັມ) floorf (ratio);
    ຈຳນວນເຕັມ b = (ຈຳນວນເຕັມ) ceilf (ratio);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    ຖ້າ (b <= 6) // 50-125
    {
      ຖ້າ (a == b) ສົ່ງຄືນ a + 1.0f;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (b == 8)
    {
      ຖ້າ (a == b) ສົ່ງຄືນ 8.0f; // 150
      a = 6;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
      ຖ້າ (a == b && a == 12) ສົ່ງຄືນ 9.0f; //200
      b = 12;
      a = 8;
    }

    float va = 50 + a * 12.5f;
    float vb = 50 + b * 12.5f;

    float ret =  a + (width - va) / (vb - va);
    ຖ້າ (a <= 6) ret += 1.0f;
    ສົ່ງຄືນ ret;
  }

  static unsigned calc_avg_char_width (const hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ຈຳນວນເຕັມ>>& hmtx_map)
  {
    unsigned num = 0;
    unsigned total_width = 0;
    ສຳລັບ (const auto& _ : hmtx_map.values_ref ())
    {
      unsigned width = _.first;
      ຖ້າ (width)
      {
        total_width += width;
        num++;
      }
    }

    ສົ່ງຄືນ num ? (unsigned) roundf ((dviguu_na) total_width / (dviguu_na) num) : 0;
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    OS2 *os2_prime = c->serializer->embed (this);
    ຖ້າ (unlikely (!os2_prime)) return_trace (false);

#ifndef HB_NO_VAR
    ຖ້າ (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = os2_prime;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP,         sTypoLineGap);
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

      ຖ້າ (os2_prime->version >= 2)
      {
        hb_barrier ();
        auto *table = & const_cast<OS2V2Tail &> (os2_prime->v2 ());
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_X_HEIGHT,                   sxHeight);
        HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_CAP_HEIGHT,                 sCapHeight);
      }

      unsigned avg_char_width = calc_avg_char_width (c->plan->hmtx_map);
      ຖ້າ (!c->serializer->check_assign (os2_prime->xAvgCharWidth, avg_char_width,
                                        HB_SERIALIZE_ERROR_INT_OVERFLOW))
        return_trace (false);
    }
#endif

    Triple *axis_range;
    ຖ້າ (c->plan->user_axes_location.has (HB_TAG ('w','g','h','t'), &axis_range))
    {
      unsigned weight_class = static_cast<unsigned> (roundf (hb_clamp (axis_range->middle, 1.0f, 1000.0f)));
      ຖ້າ (os2_prime->usWeightClass != weight_class)
        os2_prime->usWeightClass = weight_class;
    }

    ຖ້າ (c->plan->user_axes_location.has (HB_TAG ('w','d','t','h'), &axis_range))
    {
      unsigned width_class = static_cast<unsigned> (roundf (map_wdth_to_widthclass (axis_range->middle)));
      ຖ້າ (os2_prime->usWidthClass != width_class)
        os2_prime->usWidthClass = width_class;
    }

    os2_prime->usFirstCharIndex = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_prime->usLastCharIndex  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    ຖ້າ (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (true);

    _update_unicode_ranges (&c->plan->unicodes, os2_prime->ulUnicodeRange);

    return_trace (true);
  }

  void _update_unicode_ranges (const hb_set_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) const
  {
    HBUINT32 newBits[4];
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < 4; i++)
      newBits[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    ສຳລັບ (auto cp : *codepoints)
    {
      unsigned ຈຳນວນເຕັມ bit = _hb_ot_os2_get_unicode_range_bit (cp);
      ຖ້າ (bit < 128)
      {
	unsigned ຈຳນວນເຕັມ block = bit / 32;
	unsigned ຈຳນວນເຕັມ bit_in_block = bit % 32;
	unsigned ຈຳນວນເຕັມ mask = 1 << bit_in_block;
	newBits[block] = newBits[block] | mask;
      }
      ຖ້າ (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	newBits[1] = newBits[1] | (1 << 25);
      }
    }

    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & newBits[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  ga_nanaa font_page_t
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
  font_page_t get_font_page () const
  { ສົ່ງຄືນ (font_page_t) (version == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_size () const
  {
    size_t result = min_size;
    ຖ້າ (version >= 1) result = hb_unsigned_add_saturate (result, v1X.get_size ());
    ຖ້າ (version >= 2) result = hb_unsigned_add_saturate (result, v2X.get_size ());
    ຖ້າ (version >= 5) result = hb_unsigned_add_saturate (result, v5X.get_size ());
    ສົ່ງຄືນ result;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ຖ້າ (unlikely (!c->check_struct (this))) return_trace (false);
    hb_barrier ();
    ຖ້າ (unlikely (version >= 1 && !v1X.sanitize (c))) return_trace (false);
    ຖ້າ (unlikely (version >= 2 && !v2X.sanitize (c))) return_trace (false);
    ຖ້າ (unlikely (version >= 5 && !v5X.sanitize (c))) return_trace (false);
    return_trace (true);
  }

  public:
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
  HBINT16	sTypoLineGap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2V1Tail	v1X;
  OS2V2Tail	v2X;
  OS2V5Tail	v5X;
  public:
  DEFINE_SIZE_MIN (78);
};

} /* namespace OT */


#endif /* HB_OT_OS2_TABLE_HH */
