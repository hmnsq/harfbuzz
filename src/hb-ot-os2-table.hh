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

#جے_بنا_تعریف HB_OT_OS2_جدول_HH
#تعریف_کرو HB_OT_OS2_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-os2-unicode-ranges.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#ضمن "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#تعریف_کرو HB_OT_ٹیگ_OS2 HB_ٹیگ('O','S','/','2')


نطاق OT {

بنية OS2_V1_پوچھ
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT32	ul_کوڈ_ورقہ_Range1;
  HBUINT32	ul_کوڈ_ورقہ_Range2;
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية OS2_V2_پوچھ
{
  منطقي has_انکڑے () ثابت { أرجع sx_اچائی || s_Cap_اچائی; }

  ثابت OS2_V2_پوچھ * عامل -> () ثابت { أرجع هذا; }
  OS2_V2_پوچھ * عامل -> () { أرجع هذا; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBINT16	sx_اچائی;
  HBINT16	s_Cap_اچائی;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  عام:
  DEFINE_ناپ_STATIC (10);
};

بنية OS2_V5_پوچھ
{
  مضمن منطقي get_optical_ناپ (بنانشانی صحيح *ہیٹھلا, بنانشانی صحيح *اتلا) ثابت
  {
    بنانشانی صحيح ہیٹھلا_optical_ناپ = us_ہیٹھلا_Optical_نقطہ_ناپ;
    بنانشانی صحيح اتلا_optical_ناپ = us_اتلا_Optical_نقطہ_ناپ;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    إذا (ہیٹھلا_optical_ناپ < اتلا_optical_ناپ &&
	ہیٹھلا_optical_ناپ >= 1 && ہیٹھلا_optical_ناپ <= 0xFFFE &&
	اتلا_optical_ناپ >= 2 && اتلا_optical_ناپ <= 0xFFFF)
    {
      *ہیٹھلا = ہیٹھلا_optical_ناپ;
      *اتلا = اتلا_optical_ناپ;
      أرجع سچ;
    }
    أرجع خطأ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	us_ہیٹھلا_Optical_نقطہ_ناپ;
  HBUINT16	us_اتلا_Optical_نقطہ_ناپ;
  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية OS2
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_OS2;

  منطقي has_انکڑے () ثابت { أرجع us_بھار_Class || us_چوڑائی_Class || us_پہلا_Char_اشاریہ || us_آخری_Char_اشاریہ; }

  ثابت OS2_V1_پوچھ &v1 () ثابت { أرجع نسخہ >= 1 ? v1X : Null (OS2_V1_پوچھ); }
  ثابت OS2_V2_پوچھ &v2 () ثابت { أرجع نسخہ >= 2 ? v2X : Null (OS2_V2_پوچھ); }
  ثابت OS2_V5_پوچھ &v5 () ثابت { أرجع نسخہ >= 5 ? v5X : Null (OS2_V5_پوچھ); }

  تعداد selection_flag_t {
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

  منطقي        is_italic () ثابت { أرجع fsSelection & ITALIC; }
  منطقي       is_oblique () ثابت { أرجع fsSelection & OBLIQUE; }
  منطقي use_typo_metrics () ثابت { أرجع fsSelection & USE_TYPO_METRICS; }

  تعداد چوڑائی_class_t {
    FWIDTH_ULTRA_CONDENSED	= 1, /* 50% */
    FWIDTH_EXTRA_CONDENSED	= 2, /* 62.5% */
    FWIDTH_CONDENSED		= 3, /* 75% */
    FWIDTH_SEMI_CONDENSED	= 4, /* 87.5% */
    FWIDTH_عام		= 5, /* 100% */
    FWIDTH_SEMI_EXPANDED	= 6, /* 112.5% */
    FWIDTH_EXPANDED		= 7, /* 125% */
    FWIDTH_EXTRA_EXPANDED	= 8, /* 150% */
    FWIDTH_ULTRA_EXPANDED	= 9  /* 200% */
  };

  عائم get_چوڑائی () ثابت
  {
    بدل (us_چوڑائی_Class) {
    حالة FWIDTH_ULTRA_CONDENSED:أرجع 50.f;
    حالة FWIDTH_EXTRA_CONDENSED:أرجع 62.5f;
    حالة FWIDTH_CONDENSED:	أرجع 75.f;
    حالة FWIDTH_SEMI_CONDENSED:	أرجع 87.5f;
    افتراضي:
    حالة FWIDTH_عام:		أرجع 100.f;
    حالة FWIDTH_SEMI_EXPANDED:	أرجع 112.5f;
    حالة FWIDTH_EXPANDED:	أرجع 125.f;
    حالة FWIDTH_EXTRA_EXPANDED:	أرجع 150.f;
    حالة FWIDTH_ULTRA_EXPANDED:	أرجع 200.f;
    }
  }

  عائم map_wdth_منزل_widthclass(عائم چوڑائی) ثابت
  {
    إذا (چوڑائی < 50) أرجع 1.0f;
    إذا (چوڑائی > 200) أرجع 9.0f;

    عائم تناسب = (چوڑائی - 50) / 12.5f;
    صحيح a = (صحيح) floorf (تناسب);
    صحيح b = (صحيح) ceilf (تناسب);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    إذا (b <= 6) // 50-125
    {
      إذا (a == b) أرجع a + 1.0f;
    }
    وإلا إذا (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    وإلا إذا (b == 8)
    {
      إذا (a == b) أرجع 8.0f; // 150
      a = 6;
    }
    وإلا
    {
      إذا (a == b && a == 12) أرجع 9.0f; //200
      b = 12;
      a = 8;
    }

    عائم va = 50 + a * 12.5f;
    عائم vb = 50 + b * 12.5f;

    عائم ret =  a + (چوڑائی - va) / (vb - va);
    إذا (a <= 6) ret += 1.0f;
    أرجع ret;
  }

  ساكن بنانشانی calc_avg_char_چوڑائی (ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشانی, صحيح>>& hmtx_map)
  {
    بنانشانی عدد = 0;
    بنانشانی کل_چوڑائی = 0;
    لكل (ثابت خودکار& _ : hmtx_map.اقدار_ref ())
    {
      بنانشانی چوڑائی = _.پہلا;
      إذا (چوڑائی)
      {
        کل_چوڑائی += چوڑائی;
        عدد++;
      }
    }

    أرجع عدد ? (بنانشانی) roundf ((مزدوج) کل_چوڑائی / (مزدوج) عدد) : 0;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    OS2 *os2_مفرد = c->serializer->embed (هذا);
    إذا (unlikely (!os2_مفرد)) return_trace (خطأ);

#جے_بنا_تعریف HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->سروت->جدول.MVAR;
      خودکار *جدول = os2_مفرد;

      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_HORIZONTAL_سطر_GAP,         s_Typo_سطر_Gap);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_SUBSCRIPT_EM_X_ناپ,         ySubscriptXSize);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_SUBSCRIPT_EM_Y_ناپ,         ySubscriptYSize);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_SUBSCRIPT_EM_X_وتھ,       ySubscriptXOffset);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_SUBSCRIPT_EM_Y_وتھ,       ySubscriptYOffset);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_SUPERSCRIPT_EM_X_ناپ,       ySuperscriptXSize);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_SUPERSCRIPT_EM_Y_ناپ,       ySuperscriptYSize);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_SUPERSCRIPT_EM_X_وتھ,     ySuperscriptXOffset);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_SUPERSCRIPT_EM_Y_وتھ,     ySuperscriptYOffset);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_STRIKEOUT_ناپ,              y_Strikeout_ناپ);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_STRIKEOUT_وتھ,            yStrikeoutPosition);

      إذا (os2_مفرد->نسخہ >= 2)
      {
        hb_barrier ();
        خودکار *جدول = & const_cast<OS2_V2_پوچھ &> (os2_مفرد->v2 ());
        HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_X_اچائی,                   sx_اچائی);
        HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_CAP_اچائی,                 s_Cap_اچائی);
      }

      بنانشانی avg_char_چوڑائی = calc_avg_char_چوڑائی (c->plan->hmtx_map);
      إذا (!c->serializer->check_سونپو (os2_مفرد->x_Avg_Char_چوڑائی, avg_char_چوڑائی,
                                        HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
        return_trace (خطأ);
    }
#جے_انت

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_ٹیگ ('w','g','h','t'), &axis_range))
    {
      بنانشانی بھار_class = static_cast<بنانشانی> (roundf (hb_clamp (axis_range->درمیان, 1.0f, 1000.0f)));
      إذا (os2_مفرد->us_بھار_Class != بھار_class)
        os2_مفرد->us_بھار_Class = بھار_class;
    }

    إذا (c->plan->user_axes_location.has (HB_ٹیگ ('w','d','t','h'), &axis_range))
    {
      بنانشانی چوڑائی_class = static_cast<بنانشانی> (roundf (map_wdth_منزل_widthclass (axis_range->درمیان)));
      إذا (os2_مفرد->us_چوڑائی_Class != چوڑائی_class)
        os2_مفرد->us_چوڑائی_Class = چوڑائی_class;
    }

    os2_مفرد->us_پہلا_Char_اشاریہ = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_مفرد->us_آخری_Char_اشاریہ  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (سچ);

    _update_unicode_ranges (&c->plan->unicodes, os2_مفرد->ulUnicodeRange);

    return_trace (سچ);
  }

  فراغ _update_unicode_ranges (ثابت hb_سیٹ_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ثابت
  {
    HBUINT32 new_بٹ[4];
    لكل (بنانشانی صحيح i = 0; i < 4; i++)
      new_بٹ[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    لكل (خودکار cp : *codepoints)
    {
      بنانشانی صحيح bit = hb_ot_os2_get_unicode_range_ثنائی_ہندسہ (cp);
      إذا (bit < 128)
      {
	بنانشانی صحيح بلاک = bit / 32;
	بنانشانی صحيح ثنائی_ہندسہ_وچ_بلاک = bit % 32;
	بنانشانی صحيح نقاب = 1 << ثنائی_ہندسہ_وچ_بلاک;
	new_بٹ[بلاک] = new_بٹ[بلاک] | نقاب;
      }
      إذا (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	new_بٹ[1] = new_بٹ[1] | (1 << 25);
      }
    }

    لكل (بنانشانی صحيح i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & new_بٹ[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  تعداد font_ورقہ_t
  {
    FONT_ورقہ_NONE		= 0,
    FONT_ورقہ_HEBREW		= 0xB100, /* Hebrew Windows 3.1 font page */
    FONT_ورقہ_SIMP_ARABIC	= 0xB200, /* Simplified Arabic Windows 3.1 font page */
    FONT_ورقہ_TRAD_ARABIC	= 0xB300, /* Traditional Arabic Windows 3.1 font page */
    FONT_ورقہ_OEM_ARABIC	= 0xB400, /* OEM Arabic Windows 3.1 font page */
    FONT_ورقہ_SIMP_FARSI	= 0xBA00, /* Simplified Farsi Windows 3.1 font page */
    FONT_ورقہ_TRAD_FARSI	= 0xBB00, /* Traditional Farsi Windows 3.1 font page */
    FONT_ورقہ_THAI		= 0xDE00  /* Thai Windows 3.1 font page */
  };
  font_ورقہ_t get_font_ورقہ () ثابت
  { أرجع (font_ورقہ_t) (نسخہ == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_ناپ () ثابت
  {
    size_t نتیجہ = min_ناپ;
    إذا (نسخہ >= 1) نتیجہ = hb_unsigned_جوڑو_saturate (نتیجہ, v1X.get_ناپ ());
    إذا (نسخہ >= 2) نتیجہ = hb_unsigned_جوڑو_saturate (نتیجہ, v2X.get_ناپ ());
    إذا (نسخہ >= 5) نتیجہ = hb_unsigned_جوڑو_saturate (نتیجہ, v5X.get_ناپ ());
    أرجع نتیجہ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (نسخہ >= 1 && !v1X.sanitize (c))) return_trace (خطأ);
    إذا (unlikely (نسخہ >= 2 && !v2X.sanitize (c))) return_trace (خطأ);
    إذا (unlikely (نسخہ >= 5 && !v5X.sanitize (c))) return_trace (خطأ);
    return_trace (سچ);
  }

  عام:
  HBUINT16	نسخہ;
  HBINT16	x_Avg_Char_چوڑائی;
  HBUINT16	us_بھار_Class;
  HBUINT16	us_چوڑائی_Class;
  HBUINT16	fsType;
  HBINT16	ySubscriptXSize;
  HBINT16	ySubscriptYSize;
  HBINT16	ySubscriptXOffset;
  HBINT16	ySubscriptYOffset;
  HBINT16	ySuperscriptXSize;
  HBINT16	ySuperscriptYSize;
  HBINT16	ySuperscriptXOffset;
  HBINT16	ySuperscriptYOffset;
  HBINT16	y_Strikeout_ناپ;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  ٹیگ		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_پہلا_Char_اشاریہ;
  HBUINT16	us_آخری_Char_اشاریہ;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_سطر_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_پوچھ	v1X;
  OS2_V2_پوچھ	v2X;
  OS2_V5_پوچھ	v5X;
  عام:
  DEFINE_ناپ_MIN (78);
};

} /* namespace OT */


#جے_انت /* HB_OT_OS2_TABLE_HH */
