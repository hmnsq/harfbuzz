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

#اگر_غیر_تعریف_شدہ HB_OT_OS2_جدول_HH
#تعریف_کرو HB_OT_OS2_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-os2-unicode-ranges.hh"
#شامل "hb-ot-var-mvar-table.hh"

#شامل "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#تعریف_کرو HB_OT_لیبل_OS2 HB_لیبل('O','S','/','2')


نامخلا OT {

ساخت OS2_V1_دم
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT32	ul_رمز_صفحہ_Range1;
  HBUINT32	ul_رمز_صفحہ_Range2;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت OS2_V2_دم
{
  منطقی has_ڈیٹا () ثابت { واپس sx_اونچائی || s_Cap_اونچائی; }

  ثابت OS2_V2_دم * عامل -> () ثابت { واپس یہ; }
  OS2_V2_دم * عامل -> () { واپس یہ; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBINT16	sx_اونچائی;
  HBINT16	s_Cap_اونچائی;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  عوامی:
  DEFINE_سائز_STATIC (10);
};

ساخت OS2_V5_دم
{
  اندرونی منطقی get_optical_سائز (غیرعلامتی صحیح *زیریں, غیرعلامتی صحیح *بالائی) ثابت
  {
    غیرعلامتی صحیح زیریں_optical_سائز = us_زیریں_Optical_نقطہ_سائز;
    غیرعلامتی صحیح بالائی_optical_سائز = us_بالائی_Optical_نقطہ_سائز;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    اگر (زیریں_optical_سائز < بالائی_optical_سائز &&
	زیریں_optical_سائز >= 1 && زیریں_optical_سائز <= 0xFFFE &&
	بالائی_optical_سائز >= 2 && بالائی_optical_سائز <= 0xFFFF)
    {
      *زیریں = زیریں_optical_سائز;
      *بالائی = بالائی_optical_سائز;
      واپس سچ;
    }
    واپس جھوٹ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT16	us_زیریں_Optical_نقطہ_سائز;
  HBUINT16	us_بالائی_Optical_نقطہ_سائز;
  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت OS2
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_OS2;

  منطقی has_ڈیٹا () ثابت { واپس us_وزن_Class || us_چوڑائی_Class || us_پہلا_Char_اشاریہ || us_آخری_Char_اشاریہ; }

  ثابت OS2_V1_دم &v1 () ثابت { واپس نسخہ >= 1 ? v1X : Null (OS2_V1_دم); }
  ثابت OS2_V2_دم &v2 () ثابت { واپس نسخہ >= 2 ? v2X : Null (OS2_V2_دم); }
  ثابت OS2_V5_دم &v5 () ثابت { واپس نسخہ >= 5 ? v5X : Null (OS2_V5_دم); }

  شمار selection_flag_t {
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

  منطقی        is_italic () ثابت { واپس fsSelection & ITALIC; }
  منطقی       is_oblique () ثابت { واپس fsSelection & OBLIQUE; }
  منطقی use_typo_metrics () ثابت { واپس fsSelection & USE_TYPO_METRICS; }

  شمار چوڑائی_class_t {
    FWIDTH_ULTRA_CONDENSED	= 1, /* 50% */
    FWIDTH_EXTRA_CONDENSED	= 2, /* 62.5% */
    FWIDTH_CONDENSED		= 3, /* 75% */
    FWIDTH_SEMI_CONDENSED	= 4, /* 87.5% */
    FWIDTH_معمول		= 5, /* 100% */
    FWIDTH_SEMI_EXPANDED	= 6, /* 112.5% */
    FWIDTH_EXPANDED		= 7, /* 125% */
    FWIDTH_EXTRA_EXPANDED	= 8, /* 150% */
    FWIDTH_ULTRA_EXPANDED	= 9  /* 200% */
  };

  اعشاری get_چوڑائی () ثابت
  {
    انتخاب (us_چوڑائی_Class) {
    حالت FWIDTH_ULTRA_CONDENSED:واپس 50.f;
    حالت FWIDTH_EXTRA_CONDENSED:واپس 62.5f;
    حالت FWIDTH_CONDENSED:	واپس 75.f;
    حالت FWIDTH_SEMI_CONDENSED:	واپس 87.5f;
    طےشدہ:
    حالت FWIDTH_معمول:		واپس 100.f;
    حالت FWIDTH_SEMI_EXPANDED:	واپس 112.5f;
    حالت FWIDTH_EXPANDED:	واپس 125.f;
    حالت FWIDTH_EXTRA_EXPANDED:	واپس 150.f;
    حالت FWIDTH_ULTRA_EXPANDED:	واپس 200.f;
    }
  }

  اعشاری map_wdth_مقصد_widthclass(اعشاری چوڑائی) ثابت
  {
    اگر (چوڑائی < 50) واپس 1.0f;
    اگر (چوڑائی > 200) واپس 9.0f;

    اعشاری تناسب = (چوڑائی - 50) / 12.5f;
    صحیح a = (صحیح) floorf (تناسب);
    صحیح b = (صحیح) ceilf (تناسب);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    اگر (b <= 6) // 50-125
    {
      اگر (a == b) واپس a + 1.0f;
    }
    ورنہ اگر (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    ورنہ اگر (b == 8)
    {
      اگر (a == b) واپس 8.0f; // 150
      a = 6;
    }
    ورنہ
    {
      اگر (a == b && a == 12) واپس 9.0f; //200
      b = 12;
      a = 8;
    }

    اعشاری va = 50 + a * 12.5f;
    اعشاری vb = 50 + b * 12.5f;

    اعشاری ret =  a + (چوڑائی - va) / (vb - va);
    اگر (a <= 6) ret += 1.0f;
    واپس ret;
  }

  ساکن غیرعلامتی calc_avg_char_چوڑائی (ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<غیرعلامتی, صحیح>>& hmtx_map)
  {
    غیرعلامتی عدد = 0;
    غیرعلامتی کل_چوڑائی = 0;
    برائے (ثابت خودکار& _ : hmtx_map.اقدار_ref ())
    {
      غیرعلامتی چوڑائی = _.پہلا;
      اگر (چوڑائی)
      {
        کل_چوڑائی += چوڑائی;
        عدد++;
      }
    }

    واپس عدد ? (غیرعلامتی) roundf ((دوہرا) کل_چوڑائی / (دوہرا) عدد) : 0;
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    OS2 *os2_مفرد = c->serializer->embed (یہ);
    اگر (unlikely (!os2_مفرد)) return_trace (جھوٹ);

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->ماخذ->جدول.MVAR;
      خودکار *جدول = os2_مفرد;

      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP,         s_Typo_سطر_Gap);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_SUBSCRIPT_EM_X_سائز,         ySubscriptXSize);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_SUBSCRIPT_EM_Y_سائز,         ySubscriptYSize);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_SUBSCRIPT_EM_X_انحراف,       ySubscriptXOffset);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_SUBSCRIPT_EM_Y_انحراف,       ySubscriptYOffset);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_X_سائز,       ySuperscriptXSize);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_Y_سائز,       ySuperscriptYSize);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_X_انحراف,     ySuperscriptXOffset);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_Y_انحراف,     ySuperscriptYOffset);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_STRIKEOUT_سائز,              y_Strikeout_سائز);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_STRIKEOUT_انحراف,            yStrikeoutPosition);

      اگر (os2_مفرد->نسخہ >= 2)
      {
        hb_barrier ();
        خودکار *جدول = & const_cast<OS2_V2_دم &> (os2_مفرد->v2 ());
        HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_X_اونچائی,                   sx_اونچائی);
        HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_CAP_اونچائی,                 s_Cap_اونچائی);
      }

      غیرعلامتی avg_char_چوڑائی = calc_avg_char_چوڑائی (c->plan->hmtx_map);
      اگر (!c->serializer->check_تفویض (os2_مفرد->x_Avg_Char_چوڑائی, avg_char_چوڑائی,
                                        HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
        return_trace (جھوٹ);
    }
#اگر_ختم

    Triple *axis_range;
    اگر (c->plan->user_axes_location.has (HB_لیبل ('w','g','h','t'), &axis_range))
    {
      غیرعلامتی وزن_class = static_cast<غیرعلامتی> (roundf (hb_clamp (axis_range->درمیان, 1.0f, 1000.0f)));
      اگر (os2_مفرد->us_وزن_Class != وزن_class)
        os2_مفرد->us_وزن_Class = وزن_class;
    }

    اگر (c->plan->user_axes_location.has (HB_لیبل ('w','d','t','h'), &axis_range))
    {
      غیرعلامتی چوڑائی_class = static_cast<غیرعلامتی> (roundf (map_wdth_مقصد_widthclass (axis_range->درمیان)));
      اگر (os2_مفرد->us_چوڑائی_Class != چوڑائی_class)
        os2_مفرد->us_چوڑائی_Class = چوڑائی_class;
    }

    os2_مفرد->us_پہلا_Char_اشاریہ = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_مفرد->us_آخری_Char_اشاریہ  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (سچ);

    _update_unicode_ranges (&c->plan->unicodes, os2_مفرد->ulUnicodeRange);

    return_trace (سچ);
  }

  خالی _update_unicode_ranges (ثابت hb_سیٹ_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ثابت
  {
    HBUINT32 new_بٹ[4];
    برائے (غیرعلامتی صحیح i = 0; i < 4; i++)
      new_بٹ[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    برائے (خودکار cp : *codepoints)
    {
      غیرعلامتی صحیح bit = hb_ot_os2_get_unicode_range_ثنائی_ہندسہ (cp);
      اگر (bit < 128)
      {
	غیرعلامتی صحیح بلاک = bit / 32;
	غیرعلامتی صحیح ثنائی_ہندسہ_اندر_بلاک = bit % 32;
	غیرعلامتی صحیح نقاب = 1 << ثنائی_ہندسہ_اندر_بلاک;
	new_بٹ[بلاک] = new_بٹ[بلاک] | نقاب;
      }
      اگر (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	new_بٹ[1] = new_بٹ[1] | (1 << 25);
      }
    }

    برائے (غیرعلامتی صحیح i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & new_بٹ[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  شمار font_صفحہ_t
  {
    FONT_صفحہ_NONE		= 0,
    FONT_صفحہ_HEBREW		= 0xB100, /* Hebrew Windows 3.1 font page */
    FONT_صفحہ_SIMP_ARABIC	= 0xB200, /* Simplified Arabic Windows 3.1 font page */
    FONT_صفحہ_TRAD_ARABIC	= 0xB300, /* Traditional Arabic Windows 3.1 font page */
    FONT_صفحہ_OEM_ARABIC	= 0xB400, /* OEM Arabic Windows 3.1 font page */
    FONT_صفحہ_SIMP_FARSI	= 0xBA00, /* Simplified Farsi Windows 3.1 font page */
    FONT_صفحہ_TRAD_FARSI	= 0xBB00, /* Traditional Farsi Windows 3.1 font page */
    FONT_صفحہ_THAI		= 0xDE00  /* Thai Windows 3.1 font page */
  };
  font_صفحہ_t get_font_صفحہ () ثابت
  { واپس (font_صفحہ_t) (نسخہ == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_سائز () ثابت
  {
    size_t نتیجہ = min_سائز;
    اگر (نسخہ >= 1) نتیجہ = hb_unsigned_جوڑو_saturate (نتیجہ, v1X.get_سائز ());
    اگر (نسخہ >= 2) نتیجہ = hb_unsigned_جوڑو_saturate (نتیجہ, v2X.get_سائز ());
    اگر (نسخہ >= 5) نتیجہ = hb_unsigned_جوڑو_saturate (نتیجہ, v5X.get_سائز ());
    واپس نتیجہ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ))) return_trace (جھوٹ);
    hb_barrier ();
    اگر (unlikely (نسخہ >= 1 && !v1X.sanitize (c))) return_trace (جھوٹ);
    اگر (unlikely (نسخہ >= 2 && !v2X.sanitize (c))) return_trace (جھوٹ);
    اگر (unlikely (نسخہ >= 5 && !v5X.sanitize (c))) return_trace (جھوٹ);
    return_trace (سچ);
  }

  عوامی:
  HBUINT16	نسخہ;
  HBINT16	x_Avg_Char_چوڑائی;
  HBUINT16	us_وزن_Class;
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
  HBINT16	y_Strikeout_سائز;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  لیبل		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_پہلا_Char_اشاریہ;
  HBUINT16	us_آخری_Char_اشاریہ;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_سطر_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_دم	v1X;
  OS2_V2_دم	v2X;
  OS2_V5_دم	v5X;
  عوامی:
  DEFINE_سائز_MIN (78);
};

} /* namespace OT */


#اگر_ختم /* HB_OT_OS2_TABLE_HH */
