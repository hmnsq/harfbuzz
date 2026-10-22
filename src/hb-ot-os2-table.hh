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

#که_نه_تعریف_شوی HB_OT_OS2_جدول_HH
#تعریف_کړه HB_OT_OS2_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-os2-unicode-ranges.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#ضمن "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#تعریف_کړه HB_OT_نښان_OS2 HB_نښان('O','S','/','2')


نطاق OT {

بنية OS2_V1_لکۍ
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT32	ul_کوډ_پاڼه_Range1;
  HBUINT32	ul_کوډ_پاڼه_Range2;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية OS2_V2_لکۍ
{
  منطقي has_معلومات () ثابت { أرجع sx_لوړوالی || s_Cap_لوړوالی; }

  ثابت OS2_V2_لکۍ * چلوونکی -> () ثابت { أرجع هذا; }
  OS2_V2_لکۍ * چلوونکی -> () { أرجع هذا; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBINT16	sx_لوړوالی;
  HBINT16	s_Cap_لوړوالی;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  عام:
  DEFINE_اندازه_STATIC (10);
};

بنية OS2_V5_لکۍ
{
  مضمن منطقي get_optical_اندازه (بینښه صحيح *ښکتنی, بینښه صحيح *پاسنی) ثابت
  {
    بینښه صحيح ښکتنی_optical_اندازه = us_ښکتنی_Optical_ټکی_اندازه;
    بینښه صحيح پاسنی_optical_اندازه = us_پاسنی_Optical_ټکی_اندازه;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    إذا (ښکتنی_optical_اندازه < پاسنی_optical_اندازه &&
	ښکتنی_optical_اندازه >= 1 && ښکتنی_optical_اندازه <= 0xFFFE &&
	پاسنی_optical_اندازه >= 2 && پاسنی_optical_اندازه <= 0xFFFF)
    {
      *ښکتنی = ښکتنی_optical_اندازه;
      *پاسنی = پاسنی_optical_اندازه;
      أرجع ریښتیا;
    }
    أرجع خطأ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	us_ښکتنی_Optical_ټکی_اندازه;
  HBUINT16	us_پاسنی_Optical_ټکی_اندازه;
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية OS2
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_OS2;

  منطقي has_معلومات () ثابت { أرجع us_وزن_Class || us_پلنوالی_Class || us_لومړی_Char_شاخص || us_وروستی_Char_شاخص; }

  ثابت OS2_V1_لکۍ &v1 () ثابت { أرجع نسخه >= 1 ? v1X : Null (OS2_V1_لکۍ); }
  ثابت OS2_V2_لکۍ &v2 () ثابت { أرجع نسخه >= 2 ? v2X : Null (OS2_V2_لکۍ); }
  ثابت OS2_V5_لکۍ &v5 () ثابت { أرجع نسخه >= 5 ? v5X : Null (OS2_V5_لکۍ); }

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

  تعداد پلنوالی_class_t {
    FWIDTH_ULTRA_CONDENSED	= 1, /* 50% */
    FWIDTH_EXTRA_CONDENSED	= 2, /* 62.5% */
    FWIDTH_CONDENSED		= 3, /* 75% */
    FWIDTH_SEMI_CONDENSED	= 4, /* 87.5% */
    FWIDTH_نورمال		= 5, /* 100% */
    FWIDTH_SEMI_EXPANDED	= 6, /* 112.5% */
    FWIDTH_EXPANDED		= 7, /* 125% */
    FWIDTH_EXTRA_EXPANDED	= 8, /* 150% */
    FWIDTH_ULTRA_EXPANDED	= 9  /* 200% */
  };

  عائم get_پلنوالی () ثابت
  {
    بدل (us_پلنوالی_Class) {
    حالة FWIDTH_ULTRA_CONDENSED:أرجع 50.f;
    حالة FWIDTH_EXTRA_CONDENSED:أرجع 62.5f;
    حالة FWIDTH_CONDENSED:	أرجع 75.f;
    حالة FWIDTH_SEMI_CONDENSED:	أرجع 87.5f;
    افتراضي:
    حالة FWIDTH_نورمال:		أرجع 100.f;
    حالة FWIDTH_SEMI_EXPANDED:	أرجع 112.5f;
    حالة FWIDTH_EXPANDED:	أرجع 125.f;
    حالة FWIDTH_EXTRA_EXPANDED:	أرجع 150.f;
    حالة FWIDTH_ULTRA_EXPANDED:	أرجع 200.f;
    }
  }

  عائم map_wdth_منزل_widthclass(عائم پلنوالی) ثابت
  {
    إذا (پلنوالی < 50) أرجع 1.0f;
    إذا (پلنوالی > 200) أرجع 9.0f;

    عائم نسبت = (پلنوالی - 50) / 12.5f;
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

    عائم ret =  a + (پلنوالی - va) / (vb - va);
    إذا (a <= 6) ret += 1.0f;
    أرجع ret;
  }

  ساكن بینښه calc_avg_char_پلنوالی (ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بینښه, صحيح>>& hmtx_map)
  {
    بینښه عدد = 0;
    بینښه ټول_پلنوالی = 0;
    لكل (ثابت خپلکاری& _ : hmtx_map.ارزښتونه_ref ())
    {
      بینښه پلنوالی = _.لومړی;
      إذا (پلنوالی)
      {
        ټول_پلنوالی += پلنوالی;
        عدد++;
      }
    }

    أرجع عدد ? (بینښه) roundf ((مزدوج) ټول_پلنوالی / (مزدوج) عدد) : 0;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    OS2 *os2_اولي = c->serializer->embed (هذا);
    إذا (unlikely (!os2_اولي)) return_trace (خطأ);

#که_نه_تعریف_شوی HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خپلکاری &MVAR = *c->plan->سرچینه->جدول.MVAR;
      خپلکاری *جدول = os2_اولي;

      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP,         s_Typo_کرښه_Gap);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_SUBSCRIPT_EM_X_اندازه,         ySubscriptXSize);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_SUBSCRIPT_EM_Y_اندازه,         ySubscriptYSize);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_SUBSCRIPT_EM_X_دننه_واټن,       ySubscriptXOffset);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_SUBSCRIPT_EM_Y_دننه_واټن,       ySubscriptYOffset);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_SUPERSCRIPT_EM_X_اندازه,       ySuperscriptXSize);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_SUPERSCRIPT_EM_Y_اندازه,       ySuperscriptYSize);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_SUPERSCRIPT_EM_X_دننه_واټن,     ySuperscriptXOffset);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_SUPERSCRIPT_EM_Y_دننه_واټن,     ySuperscriptYOffset);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_STRIKEOUT_اندازه,              y_Strikeout_اندازه);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_STRIKEOUT_دننه_واټن,            yStrikeoutPosition);

      إذا (os2_اولي->نسخه >= 2)
      {
        hb_barrier ();
        خپلکاری *جدول = & const_cast<OS2_V2_لکۍ &> (os2_اولي->v2 ());
        HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_X_لوړوالی,                   sx_لوړوالی);
        HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_CAP_لوړوالی,                 s_Cap_لوړوالی);
      }

      بینښه avg_char_پلنوالی = calc_avg_char_پلنوالی (c->plan->hmtx_map);
      إذا (!c->serializer->check_ټاکل (os2_اولي->x_Avg_Char_پلنوالی, avg_char_پلنوالی,
                                        HB_SERIALIZE_ERROR_INT_ډکېدنه))
        return_trace (خطأ);
    }
#د_که_پای

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_نښان ('w','g','h','t'), &axis_range))
    {
      بینښه وزن_class = static_cast<بینښه> (roundf (hb_clamp (axis_range->منځ, 1.0f, 1000.0f)));
      إذا (os2_اولي->us_وزن_Class != وزن_class)
        os2_اولي->us_وزن_Class = وزن_class;
    }

    إذا (c->plan->user_axes_location.has (HB_نښان ('w','d','t','h'), &axis_range))
    {
      بینښه پلنوالی_class = static_cast<بینښه> (roundf (map_wdth_منزل_widthclass (axis_range->منځ)));
      إذا (os2_اولي->us_پلنوالی_Class != پلنوالی_class)
        os2_اولي->us_پلنوالی_Class = پلنوالی_class;
    }

    os2_اولي->us_لومړی_Char_شاخص = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_اولي->us_وروستی_Char_شاخص  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (ریښتیا);

    _update_unicode_ranges (&c->plan->unicodes, os2_اولي->ulUnicodeRange);

    return_trace (ریښتیا);
  }

  فراغ _update_unicode_ranges (ثابت hb_سېټ_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ثابت
  {
    HBUINT32 new_بټونه[4];
    لكل (بینښه صحيح i = 0; i < 4; i++)
      new_بټونه[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    لكل (خپلکاری cp : *codepoints)
    {
      بینښه صحيح bit = hb_ot_os2_get_unicode_range_بټ (cp);
      إذا (bit < 128)
      {
	بینښه صحيح بلاک = bit / 32;
	بینښه صحيح بټ_دننه_بلاک = bit % 32;
	بینښه صحيح ماسک = 1 << بټ_دننه_بلاک;
	new_بټونه[بلاک] = new_بټونه[بلاک] | ماسک;
      }
      إذا (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	new_بټونه[1] = new_بټونه[1] | (1 << 25);
      }
    }

    لكل (بینښه صحيح i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & new_بټونه[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  تعداد font_پاڼه_t
  {
    FONT_پاڼه_NONE		= 0,
    FONT_پاڼه_HEBREW		= 0xB100, /* Hebrew Windows 3.1 font page */
    FONT_پاڼه_SIMP_ARABIC	= 0xB200, /* Simplified Arabic Windows 3.1 font page */
    FONT_پاڼه_TRAD_ARABIC	= 0xB300, /* Traditional Arabic Windows 3.1 font page */
    FONT_پاڼه_OEM_ARABIC	= 0xB400, /* OEM Arabic Windows 3.1 font page */
    FONT_پاڼه_SIMP_FARSI	= 0xBA00, /* Simplified Farsi Windows 3.1 font page */
    FONT_پاڼه_TRAD_FARSI	= 0xBB00, /* Traditional Farsi Windows 3.1 font page */
    FONT_پاڼه_THAI		= 0xDE00  /* Thai Windows 3.1 font page */
  };
  font_پاڼه_t get_font_پاڼه () ثابت
  { أرجع (font_پاڼه_t) (نسخه == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_اندازه () ثابت
  {
    size_t پایله = min_اندازه;
    إذا (نسخه >= 1) پایله = hb_unsigned_جمع_کړه_saturate (پایله, v1X.get_اندازه ());
    إذا (نسخه >= 2) پایله = hb_unsigned_جمع_کړه_saturate (پایله, v2X.get_اندازه ());
    إذا (نسخه >= 5) پایله = hb_unsigned_جمع_کړه_saturate (پایله, v5X.get_اندازه ());
    أرجع پایله;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (نسخه >= 1 && !v1X.sanitize (c))) return_trace (خطأ);
    إذا (unlikely (نسخه >= 2 && !v2X.sanitize (c))) return_trace (خطأ);
    إذا (unlikely (نسخه >= 5 && !v5X.sanitize (c))) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  عام:
  HBUINT16	نسخه;
  HBINT16	x_Avg_Char_پلنوالی;
  HBUINT16	us_وزن_Class;
  HBUINT16	us_پلنوالی_Class;
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
  نښان		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_لومړی_Char_شاخص;
  HBUINT16	us_وروستی_Char_شاخص;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_کرښه_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_لکۍ	v1X;
  OS2_V2_لکۍ	v2X;
  OS2_V5_لکۍ	v5X;
  عام:
  DEFINE_اندازه_MIN (78);
};

} /* namespace OT */


#د_که_پای /* HB_OT_OS2_TABLE_HH */
