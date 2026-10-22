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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_OS2_جدول_HH
#وصف_ڪريو HB_OT_OS2_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-os2-unicode-ranges.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#ضمن "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#وصف_ڪريو HB_OT_ٽيگ_OS2 HB_ٽيگ('O','S','/','2')


نطاق OT {

بنية OS2_V1_پڇ
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT32	ul_ڪوڊ_صفحو_Range1;
  HBUINT32	ul_ڪوڊ_صفحو_Range2;
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية OS2_V2_پڇ
{
  منطقي has_معلومات () ثابت { أرجع sx_اوچائي || s_Cap_اوچائي; }

  ثابت OS2_V2_پڇ * عامل -> () ثابت { أرجع هذا; }
  OS2_V2_پڇ * عامل -> () { أرجع هذا; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBINT16	sx_اوچائي;
  HBINT16	s_Cap_اوچائي;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  عام:
  DEFINE_سائيز_STATIC (10);
};

بنية OS2_V5_پڇ
{
  مضمن منطقي get_optical_سائيز (بنانشان صحيح *هيٺاهون, بنانشان صحيح *مٿانهون) ثابت
  {
    بنانشان صحيح هيٺاهون_optical_سائيز = us_هيٺاهون_Optical_نقطو_سائيز;
    بنانشان صحيح مٿانهون_optical_سائيز = us_مٿانهون_Optical_نقطو_سائيز;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    إذا (هيٺاهون_optical_سائيز < مٿانهون_optical_سائيز &&
	هيٺاهون_optical_سائيز >= 1 && هيٺاهون_optical_سائيز <= 0xFFFE &&
	مٿانهون_optical_سائيز >= 2 && مٿانهون_optical_سائيز <= 0xFFFF)
    {
      *هيٺاهون = هيٺاهون_optical_سائيز;
      *مٿانهون = مٿانهون_optical_سائيز;
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
  HBUINT16	us_هيٺاهون_Optical_نقطو_سائيز;
  HBUINT16	us_مٿانهون_Optical_نقطو_سائيز;
  عام:
  DEFINE_سائيز_STATIC (4);
};

بنية OS2
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_OS2;

  منطقي has_معلومات () ثابت { أرجع us_وزن_Class || us_ويڪر_Class || us_پهريون_Char_اشاريو || us_آخري_Char_اشاريو; }

  ثابت OS2_V1_پڇ &v1 () ثابت { أرجع نسخو >= 1 ? v1X : Null (OS2_V1_پڇ); }
  ثابت OS2_V2_پڇ &v2 () ثابت { أرجع نسخو >= 2 ? v2X : Null (OS2_V2_پڇ); }
  ثابت OS2_V5_پڇ &v5 () ثابت { أرجع نسخو >= 5 ? v5X : Null (OS2_V5_پڇ); }

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

  تعداد ويڪر_class_t {
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

  عائم get_ويڪر () ثابت
  {
    بدل (us_ويڪر_Class) {
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

  عائم map_wdth_منزل_widthclass(عائم ويڪر) ثابت
  {
    إذا (ويڪر < 50) أرجع 1.0f;
    إذا (ويڪر > 200) أرجع 9.0f;

    عائم تناسب = (ويڪر - 50) / 12.5f;
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

    عائم ret =  a + (ويڪر - va) / (vb - va);
    إذا (a <= 6) ret += 1.0f;
    أرجع ret;
  }

  ساكن بنانشان calc_avg_char_ويڪر (ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشان, صحيح>>& hmtx_map)
  {
    بنانشان انگ = 0;
    بنانشان ڪل_ويڪر = 0;
    لكل (ثابت خودڪار& _ : hmtx_map.قدرون_ref ())
    {
      بنانشان ويڪر = _.پهريون;
      إذا (ويڪر)
      {
        ڪل_ويڪر += ويڪر;
        انگ++;
      }
    }

    أرجع انگ ? (بنانشان) roundf ((مزدوج) ڪل_ويڪر / (مزدوج) انگ) : 0;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    OS2 *os2_مفرد = c->serializer->embed (هذا);
    إذا (unlikely (!os2_مفرد)) return_trace (خطأ);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودڪار &MVAR = *c->plan->ذريعو->جدول.MVAR;
      خودڪار *جدول = os2_مفرد;

      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP,         s_Typo_سٽ_Gap);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_X_سائيز,         ySubscriptXSize);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_Y_سائيز,         ySubscriptYSize);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_X_وٿي,       ySubscriptXOffset);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_Y_وٿي,       ySubscriptYOffset);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_X_سائيز,       ySuperscriptXSize);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_Y_سائيز,       ySuperscriptYSize);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_X_وٿي,     ySuperscriptXOffset);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_Y_وٿي,     ySuperscriptYOffset);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_STRIKEOUT_سائيز,              y_Strikeout_سائيز);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_STRIKEOUT_وٿي,            yStrikeoutPosition);

      إذا (os2_مفرد->نسخو >= 2)
      {
        hb_barrier ();
        خودڪار *جدول = & const_cast<OS2_V2_پڇ &> (os2_مفرد->v2 ());
        HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_X_اوچائي,                   sx_اوچائي);
        HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_CAP_اوچائي,                 s_Cap_اوچائي);
      }

      بنانشان avg_char_ويڪر = calc_avg_char_ويڪر (c->plan->hmtx_map);
      إذا (!c->serializer->check_سونپيو (os2_مفرد->x_Avg_Char_ويڪر, avg_char_ويڪر,
                                        HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
        return_trace (خطأ);
    }
#جيڪڏهن_پورو

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_ٽيگ ('w','g','h','t'), &axis_range))
    {
      بنانشان وزن_class = static_cast<بنانشان> (roundf (hb_clamp (axis_range->وچ, 1.0f, 1000.0f)));
      إذا (os2_مفرد->us_وزن_Class != وزن_class)
        os2_مفرد->us_وزن_Class = وزن_class;
    }

    إذا (c->plan->user_axes_location.has (HB_ٽيگ ('w','d','t','h'), &axis_range))
    {
      بنانشان ويڪر_class = static_cast<بنانشان> (roundf (map_wdth_منزل_widthclass (axis_range->وچ)));
      إذا (os2_مفرد->us_ويڪر_Class != ويڪر_class)
        os2_مفرد->us_ويڪر_Class = ويڪر_class;
    }

    os2_مفرد->us_پهريون_Char_اشاريو = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_مفرد->us_آخري_Char_اشاريو  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (سچ);

    _update_unicode_ranges (&c->plan->unicodes, os2_مفرد->ulUnicodeRange);

    return_trace (سچ);
  }

  فراغ _update_unicode_ranges (ثابت hb_سيٽ_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ثابت
  {
    HBUINT32 new_بٽ[4];
    لكل (بنانشان صحيح i = 0; i < 4; i++)
      new_بٽ[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    لكل (خودڪار cp : *codepoints)
    {
      بنانشان صحيح bit = hb_ot_os2_get_unicode_range_ٻٽو_انگ (cp);
      إذا (bit < 128)
      {
	بنانشان صحيح بلاڪ = bit / 32;
	بنانشان صحيح ٻٽو_انگ_اندريون_بلاڪ = bit % 32;
	بنانشان صحيح نقاب = 1 << ٻٽو_انگ_اندريون_بلاڪ;
	new_بٽ[بلاڪ] = new_بٽ[بلاڪ] | نقاب;
      }
      إذا (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	new_بٽ[1] = new_بٽ[1] | (1 << 25);
      }
    }

    لكل (بنانشان صحيح i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & new_بٽ[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  تعداد font_صفحو_t
  {
    FONT_صفحو_NONE		= 0,
    FONT_صفحو_HEBREW		= 0xB100, /* Hebrew Windows 3.1 font page */
    FONT_صفحو_SIMP_ARABIC	= 0xB200, /* Simplified Arabic Windows 3.1 font page */
    FONT_صفحو_TRAD_ARABIC	= 0xB300, /* Traditional Arabic Windows 3.1 font page */
    FONT_صفحو_OEM_ARABIC	= 0xB400, /* OEM Arabic Windows 3.1 font page */
    FONT_صفحو_SIMP_FARSI	= 0xBA00, /* Simplified Farsi Windows 3.1 font page */
    FONT_صفحو_TRAD_FARSI	= 0xBB00, /* Traditional Farsi Windows 3.1 font page */
    FONT_صفحو_THAI		= 0xDE00  /* Thai Windows 3.1 font page */
  };
  font_صفحو_t get_font_صفحو () ثابت
  { أرجع (font_صفحو_t) (نسخو == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_سائيز () ثابت
  {
    size_t نتيجو = min_سائيز;
    إذا (نسخو >= 1) نتيجو = hb_unsigned_جوڙيو_saturate (نتيجو, v1X.get_سائيز ());
    إذا (نسخو >= 2) نتيجو = hb_unsigned_جوڙيو_saturate (نتيجو, v2X.get_سائيز ());
    إذا (نسخو >= 5) نتيجو = hb_unsigned_جوڙيو_saturate (نتيجو, v5X.get_سائيز ());
    أرجع نتيجو;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (نسخو >= 1 && !v1X.sanitize (c))) return_trace (خطأ);
    إذا (unlikely (نسخو >= 2 && !v2X.sanitize (c))) return_trace (خطأ);
    إذا (unlikely (نسخو >= 5 && !v5X.sanitize (c))) return_trace (خطأ);
    return_trace (سچ);
  }

  عام:
  HBUINT16	نسخو;
  HBINT16	x_Avg_Char_ويڪر;
  HBUINT16	us_وزن_Class;
  HBUINT16	us_ويڪر_Class;
  HBUINT16	fsType;
  HBINT16	ySubscriptXSize;
  HBINT16	ySubscriptYSize;
  HBINT16	ySubscriptXOffset;
  HBINT16	ySubscriptYOffset;
  HBINT16	ySuperscriptXSize;
  HBINT16	ySuperscriptYSize;
  HBINT16	ySuperscriptXOffset;
  HBINT16	ySuperscriptYOffset;
  HBINT16	y_Strikeout_سائيز;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  ٽيگ		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_پهريون_Char_اشاريو;
  HBUINT16	us_آخري_Char_اشاريو;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_سٽ_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_پڇ	v1X;
  OS2_V2_پڇ	v2X;
  OS2_V5_پڇ	v5X;
  عام:
  DEFINE_سائيز_MIN (78);
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_OS2_TABLE_HH */
