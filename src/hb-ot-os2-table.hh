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

#اگر_تعریف_نشده HB_OT_OS2_جدول_HH
#تعریف_کن HB_OT_OS2_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-os2-unicode-ranges.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#ضمن "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#تعریف_کن HB_OT_برچسب_OS2 HB_برچسب('O','S','/','2')


نطاق OT {

بنية OS2_V1_دم
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT32	ul_کد_صفحه_Range1;
  HBUINT32	ul_کد_صفحه_Range2;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية OS2_V2_دم
{
  منطقي has_داده () ثابت { أرجع sx_ارتفاع || s_Cap_ارتفاع; }

  ثابت OS2_V2_دم * عملگر -> () ثابت { أرجع هذا; }
  OS2_V2_دم * عملگر -> () { أرجع هذا; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBINT16	sx_ارتفاع;
  HBINT16	s_Cap_ارتفاع;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  عام:
  DEFINE_اندازه_STATIC (10);
};

بنية OS2_V5_دم
{
  مضمن منطقي get_optical_اندازه (بیعلامت صحيح *پایینی, بیعلامت صحيح *فوقانی) ثابت
  {
    بیعلامت صحيح پایینی_optical_اندازه = us_پایینی_Optical_نقطه_اندازه;
    بیعلامت صحيح فوقانی_optical_اندازه = us_فوقانی_Optical_نقطه_اندازه;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    إذا (پایینی_optical_اندازه < فوقانی_optical_اندازه &&
	پایینی_optical_اندازه >= 1 && پایینی_optical_اندازه <= 0xFFFE &&
	فوقانی_optical_اندازه >= 2 && فوقانی_optical_اندازه <= 0xFFFF)
    {
      *پایینی = پایینی_optical_اندازه;
      *فوقانی = فوقانی_optical_اندازه;
      أرجع درست;
    }
    أرجع خطأ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	us_پایینی_Optical_نقطه_اندازه;
  HBUINT16	us_فوقانی_Optical_نقطه_اندازه;
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية OS2
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_OS2;

  منطقي has_داده () ثابت { أرجع us_وزن_Class || us_عرض_Class || us_اولی_Char_شاخص || us_آخرین_Char_شاخص; }

  ثابت OS2_V1_دم &v1 () ثابت { أرجع نسخه >= 1 ? v1X : Null (OS2_V1_دم); }
  ثابت OS2_V2_دم &v2 () ثابت { أرجع نسخه >= 2 ? v2X : Null (OS2_V2_دم); }
  ثابت OS2_V5_دم &v5 () ثابت { أرجع نسخه >= 5 ? v5X : Null (OS2_V5_دم); }

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

  تعداد عرض_class_t {
    FWIDTH_ULTRA_CONDENSED	= 1, /* 50% */
    FWIDTH_EXTRA_CONDENSED	= 2, /* 62.5% */
    FWIDTH_CONDENSED		= 3, /* 75% */
    FWIDTH_SEMI_CONDENSED	= 4, /* 87.5% */
    FWIDTH_نرمال		= 5, /* 100% */
    FWIDTH_SEMI_EXPANDED	= 6, /* 112.5% */
    FWIDTH_EXPANDED		= 7, /* 125% */
    FWIDTH_EXTRA_EXPANDED	= 8, /* 150% */
    FWIDTH_ULTRA_EXPANDED	= 9  /* 200% */
  };

  عائم get_عرض () ثابت
  {
    بدل (us_عرض_Class) {
    حالة FWIDTH_ULTRA_CONDENSED:أرجع 50.f;
    حالة FWIDTH_EXTRA_CONDENSED:أرجع 62.5f;
    حالة FWIDTH_CONDENSED:	أرجع 75.f;
    حالة FWIDTH_SEMI_CONDENSED:	أرجع 87.5f;
    افتراضي:
    حالة FWIDTH_نرمال:		أرجع 100.f;
    حالة FWIDTH_SEMI_EXPANDED:	أرجع 112.5f;
    حالة FWIDTH_EXPANDED:	أرجع 125.f;
    حالة FWIDTH_EXTRA_EXPANDED:	أرجع 150.f;
    حالة FWIDTH_ULTRA_EXPANDED:	أرجع 200.f;
    }
  }

  عائم map_wdth_مقصد_widthclass(عائم عرض) ثابت
  {
    إذا (عرض < 50) أرجع 1.0f;
    إذا (عرض > 200) أرجع 9.0f;

    عائم نسبت = (عرض - 50) / 12.5f;
    صحيح a = (صحيح) floorf (نسبت);
    صحيح b = (صحيح) ceilf (نسبت);

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

    عائم ret =  a + (عرض - va) / (vb - va);
    إذا (a <= 6) ret += 1.0f;
    أرجع ret;
  }

  ساكن بیعلامت calc_avg_char_عرض (ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بیعلامت, صحيح>>& hmtx_map)
  {
    بیعلامت رقم = 0;
    بیعلامت مجموع_عرض = 0;
    لكل (ثابت خودکار& _ : hmtx_map.مقادیر_ref ())
    {
      بیعلامت عرض = _.اولی;
      إذا (عرض)
      {
        مجموع_عرض += عرض;
        رقم++;
      }
    }

    أرجع رقم ? (بیعلامت) roundf ((مزدوج) مجموع_عرض / (مزدوج) رقم) : 0;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    OS2 *os2_اول = c->serializer->embed (هذا);
    إذا (unlikely (!os2_اول)) return_trace (خطأ);

#اگر_تعریف_نشده HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->منبع->جدول.MVAR;
      خودکار *جدول = os2_اول;

      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_HORIZONTAL_سطر_GAP,         s_Typo_سطر_Gap);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_SUBSCRIPT_EM_X_اندازه,         ySubscriptXSize);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_SUBSCRIPT_EM_Y_اندازه,         ySubscriptYSize);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_SUBSCRIPT_EM_X_جابه_جایی,       ySubscriptXOffset);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_SUBSCRIPT_EM_Y_جابه_جایی,       ySubscriptYOffset);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_SUPERSCRIPT_EM_X_اندازه,       ySuperscriptXSize);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_SUPERSCRIPT_EM_Y_اندازه,       ySuperscriptYSize);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_SUPERSCRIPT_EM_X_جابه_جایی,     ySuperscriptXOffset);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_SUPERSCRIPT_EM_Y_جابه_جایی,     ySuperscriptYOffset);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_STRIKEOUT_اندازه,              y_Strikeout_اندازه);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_STRIKEOUT_جابه_جایی,            yStrikeoutPosition);

      إذا (os2_اول->نسخه >= 2)
      {
        hb_barrier ();
        خودکار *جدول = & const_cast<OS2_V2_دم &> (os2_اول->v2 ());
        HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_X_ارتفاع,                   sx_ارتفاع);
        HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_CAP_ارتفاع,                 s_Cap_ارتفاع);
      }

      بیعلامت avg_char_عرض = calc_avg_char_عرض (c->plan->hmtx_map);
      إذا (!c->serializer->check_تخصیص_بده (os2_اول->x_Avg_Char_عرض, avg_char_عرض,
                                        HB_SERIALIZE_ERROR_INT_سرریز))
        return_trace (خطأ);
    }
#پایان_اگر

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_برچسب ('w','g','h','t'), &axis_range))
    {
      بیعلامت وزن_class = static_cast<بیعلامت> (roundf (hb_clamp (axis_range->میانه, 1.0f, 1000.0f)));
      إذا (os2_اول->us_وزن_Class != وزن_class)
        os2_اول->us_وزن_Class = وزن_class;
    }

    إذا (c->plan->user_axes_location.has (HB_برچسب ('w','d','t','h'), &axis_range))
    {
      بیعلامت عرض_class = static_cast<بیعلامت> (roundf (map_wdth_مقصد_widthclass (axis_range->میانه)));
      إذا (os2_اول->us_عرض_Class != عرض_class)
        os2_اول->us_عرض_Class = عرض_class;
    }

    os2_اول->us_اولی_Char_شاخص = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_اول->us_آخرین_Char_شاخص  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (درست);

    _update_unicode_ranges (&c->plan->unicodes, os2_اول->ulUnicodeRange);

    return_trace (درست);
  }

  فراغ _update_unicode_ranges (ثابت hb_مجموعه_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ثابت
  {
    HBUINT32 new_بیتها[4];
    لكل (بیعلامت صحيح i = 0; i < 4; i++)
      new_بیتها[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    لكل (خودکار cp : *codepoints)
    {
      بیعلامت صحيح bit = hb_ot_os2_get_unicode_range_بیت (cp);
      إذا (bit < 128)
      {
	بیعلامت صحيح بلوک = bit / 32;
	بیعلامت صحيح بیت_درون_بلوک = bit % 32;
	بیعلامت صحيح ماسک = 1 << بیت_درون_بلوک;
	new_بیتها[بلوک] = new_بیتها[بلوک] | ماسک;
      }
      إذا (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	new_بیتها[1] = new_بیتها[1] | (1 << 25);
      }
    }

    لكل (بیعلامت صحيح i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & new_بیتها[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  تعداد font_صفحه_t
  {
    FONT_صفحه_NONE		= 0,
    FONT_صفحه_HEBREW		= 0xB100, /* Hebrew Windows 3.1 font page */
    FONT_صفحه_SIMP_ARABIC	= 0xB200, /* Simplified Arabic Windows 3.1 font page */
    FONT_صفحه_TRAD_ARABIC	= 0xB300, /* Traditional Arabic Windows 3.1 font page */
    FONT_صفحه_OEM_ARABIC	= 0xB400, /* OEM Arabic Windows 3.1 font page */
    FONT_صفحه_SIMP_FARSI	= 0xBA00, /* Simplified Farsi Windows 3.1 font page */
    FONT_صفحه_TRAD_FARSI	= 0xBB00, /* Traditional Farsi Windows 3.1 font page */
    FONT_صفحه_THAI		= 0xDE00  /* Thai Windows 3.1 font page */
  };
  font_صفحه_t get_font_صفحه () ثابت
  { أرجع (font_صفحه_t) (نسخه == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_اندازه () ثابت
  {
    size_t نتیجه = min_اندازه;
    إذا (نسخه >= 1) نتیجه = hb_unsigned_جمع_کن_saturate (نتیجه, v1X.get_اندازه ());
    إذا (نسخه >= 2) نتیجه = hb_unsigned_جمع_کن_saturate (نتیجه, v2X.get_اندازه ());
    إذا (نسخه >= 5) نتیجه = hb_unsigned_جمع_کن_saturate (نتیجه, v5X.get_اندازه ());
    أرجع نتیجه;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (نسخه >= 1 && !v1X.sanitize (c))) return_trace (خطأ);
    إذا (unlikely (نسخه >= 2 && !v2X.sanitize (c))) return_trace (خطأ);
    إذا (unlikely (نسخه >= 5 && !v5X.sanitize (c))) return_trace (خطأ);
    return_trace (درست);
  }

  عام:
  HBUINT16	نسخه;
  HBINT16	x_Avg_Char_عرض;
  HBUINT16	us_وزن_Class;
  HBUINT16	us_عرض_Class;
  HBUINT16	fsType;
  HBINT16	ySubscriptXSize;
  HBINT16	ySubscriptYSize;
  HBINT16	ySubscriptXOffset;
  HBINT16	ySubscriptYOffset;
  HBINT16	ySuperscriptXSize;
  HBINT16	ySuperscriptYSize;
  HBINT16	ySuperscriptXOffset;
  HBINT16	ySuperscriptYOffset;
  HBINT16	y_Strikeout_اندازه;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  برچسب		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_اولی_Char_شاخص;
  HBUINT16	us_آخرین_Char_شاخص;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_سطر_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_دم	v1X;
  OS2_V2_دم	v2X;
  OS2_V5_دم	v5X;
  عام:
  DEFINE_اندازه_MIN (78);
};

} /* namespace OT */


#پایان_اگر /* HB_OT_OS2_TABLE_HH */
