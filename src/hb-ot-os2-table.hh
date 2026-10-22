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

#నిర్వచితం_కాకపోతే HB_OT_OS2_పట్టిక_HH
#నిర్వచించు HB_OT_OS2_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-os2-unicode-ranges.hh"
#అన్తర్భావ "hb-ot-var-mvar-table.hh"

#అన్తర్భావ "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#నిర్వచించు HB_OT_ట్యాగ్_OS2 HB_ట్యాగ్('O','S','/','2')


నామాకాశ OT {

నిర్మాణం OS2_V1_తోక
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT32	ul_సంకేతం_పుట_Range1;
  HBUINT32	ul_సంకేతం_పుట_Range2;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం OS2_V2_తోక
{
  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి sx_ఎత్తు || s_Cap_ఎత్తు; }

  స్థిరరాశి OS2_V2_తోక * క్రియాకారకం -> () స్థిరరాశి { తిరిగి ఇది; }
  OS2_V2_తోక * క్రియాకారకం -> () { తిరిగి ఇది; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBINT16	sx_ఎత్తు;
  HBINT16	s_Cap_ఎత్తు;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};

నిర్మాణం OS2_V5_తోక
{
  అంతఃవరుస తార్కిక get_optical_కొలత (గుర్తులేని పూర్ణాంకం *క్రింది, గుర్తులేని పూర్ణాంకం *పై) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం క్రింది_optical_కొలత = us_క్రింది_Optical_బిందువు_కొలత;
    గుర్తులేని పూర్ణాంకం పై_optical_కొలత = us_పై_Optical_బిందువు_కొలత;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    ఒకవేళ (క్రింది_optical_కొలత < పై_optical_కొలత &&
	క్రింది_optical_కొలత >= 1 && క్రింది_optical_కొలత <= 0xFFFE &&
	పై_optical_కొలత >= 2 && పై_optical_కొలత <= 0xFFFF)
    {
      *క్రింది = క్రింది_optical_కొలత;
      *పై = పై_optical_కొలత;
      తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT16	us_క్రింది_Optical_బిందువు_కొలత;
  HBUINT16	us_పై_Optical_బిందువు_కొలత;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం OS2
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_OS2;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి us_బరువు_Class || us_వెడల్పు_Class || us_మొదటి_Char_సూచీ || us_చివరి_Char_సూచీ; }

  స్థిరరాశి OS2_V1_తోక &v1 () స్థిరరాశి { తిరిగి సంస్కరణ >= 1 ? v1X : Null (OS2_V1_తోక); }
  స్థిరరాశి OS2_V2_తోక &v2 () స్థిరరాశి { తిరిగి సంస్కరణ >= 2 ? v2X : Null (OS2_V2_తోక); }
  స్థిరరాశి OS2_V5_తోక &v5 () స్థిరరాశి { తిరిగి సంస్కరణ >= 5 ? v5X : Null (OS2_V5_తోక); }

  గణన selection_flag_t {
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

  తార్కిక        is_italic () స్థిరరాశి { తిరిగి fsSelection & ITALIC; }
  తార్కిక       is_oblique () స్థిరరాశి { తిరిగి fsSelection & OBLIQUE; }
  తార్కిక use_typo_metrics () స్థిరరాశి { తిరిగి fsSelection & USE_TYPO_METRICS; }

  గణన వెడల్పు_class_t {
    FWIDTH_ULTRA_CONDENSED	= 1, /* 50% */
    FWIDTH_EXTRA_CONDENSED	= 2, /* 62.5% */
    FWIDTH_CONDENSED		= 3, /* 75% */
    FWIDTH_SEMI_CONDENSED	= 4, /* 87.5% */
    FWIDTH_సాధారణ		= 5, /* 100% */
    FWIDTH_SEMI_EXPANDED	= 6, /* 112.5% */
    FWIDTH_EXPANDED		= 7, /* 125% */
    FWIDTH_EXTRA_EXPANDED	= 8, /* 150% */
    FWIDTH_ULTRA_EXPANDED	= 9  /* 200% */
  };

  దశాంశం get_వెడల్పు () స్థిరరాశి
  {
    ఎంపిక (us_వెడల్పు_Class) {
    స్థితి FWIDTH_ULTRA_CONDENSED:తిరిగి 50.f;
    స్థితి FWIDTH_EXTRA_CONDENSED:తిరిగి 62.5f;
    స్థితి FWIDTH_CONDENSED:	తిరిగి 75.f;
    స్థితి FWIDTH_SEMI_CONDENSED:	తిరిగి 87.5f;
    అప్రమేయ:
    స్థితి FWIDTH_సాధారణ:		తిరిగి 100.f;
    స్థితి FWIDTH_SEMI_EXPANDED:	తిరిగి 112.5f;
    స్థితి FWIDTH_EXPANDED:	తిరిగి 125.f;
    స్థితి FWIDTH_EXTRA_EXPANDED:	తిరిగి 150.f;
    స్థితి FWIDTH_ULTRA_EXPANDED:	తిరిగి 200.f;
    }
  }

  దశాంశం map_wdth_గమ్యం_widthclass(దశాంశం వెడల్పు) స్థిరరాశి
  {
    ఒకవేళ (వెడల్పు < 50) తిరిగి 1.0f;
    ఒకవేళ (వెడల్పు > 200) తిరిగి 9.0f;

    దశాంశం నిష్పత్తి = (వెడల్పు - 50) / 12.5f;
    పూర్ణాంకం a = (పూర్ణాంకం) floorf (నిష్పత్తి);
    పూర్ణాంకం b = (పూర్ణాంకం) ceilf (నిష్పత్తి);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    ఒకవేళ (b <= 6) // 50-125
    {
      ఒకవేళ (a == b) తిరిగి a + 1.0f;
    }
    లేకపోతే ఒకవేళ (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    లేకపోతే ఒకవేళ (b == 8)
    {
      ఒకవేళ (a == b) తిరిగి 8.0f; // 150
      a = 6;
    }
    లేకపోతే
    {
      ఒకవేళ (a == b && a == 12) తిరిగి 9.0f; //200
      b = 12;
      a = 8;
    }

    దశాంశం va = 50 + a * 12.5f;
    దశాంశం vb = 50 + b * 12.5f;

    దశాంశం ret =  a + (వెడల్పు - va) / (vb - va);
    ఒకవేళ (a <= 6) ret += 1.0f;
    తిరిగి ret;
  }

  స్థిర గుర్తులేని calc_avg_char_వెడల్పు (స్థిరరాశి hb_hashmap_t<hb_codepoint_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>>& hmtx_map)
  {
    గుర్తులేని అంకె = 0;
    గుర్తులేని మొత్తం_వెడల్పు = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : hmtx_map.విలువలు_ref ())
    {
      గుర్తులేని వెడల్పు = _.మొదటి;
      ఒకవేళ (వెడల్పు)
      {
        మొత్తం_వెడల్పు += వెడల్పు;
        అంకె++;
      }
    }

    తిరిగి అంకె ? (గుర్తులేని) roundf ((ద్విగుణ) మొత్తం_వెడల్పు / (ద్విగుణ) అంకె) : 0;
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    OS2 *os2_ప్రధానసంఖ్య = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!os2_ప్రధానసంఖ్య)) return_trace (అబద్ధం);

#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (c->plan->normalized_coords)
    {
      స్వయంచాలక &MVAR = *c->plan->ఆధారం->పట్టిక.MVAR;
      స్వయంచాలక *పట్టిక = os2_ప్రధానసంఖ్య;

      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP,         s_Typo_పంక్తి_Gap);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_X_కొలత,         ySubscriptXSize);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_Y_కొలత,         ySubscriptYSize);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_X_స్థానభ్రంశం,       ySubscriptXOffset);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_Y_స్థానభ్రంశం,       ySubscriptYOffset);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_X_కొలత,       ySuperscriptXSize);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_Y_కొలత,       ySuperscriptYSize);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_X_స్థానభ్రంశం,     ySuperscriptXOffset);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_Y_స్థానభ్రంశం,     ySuperscriptYOffset);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_STRIKEOUT_కొలత,              y_Strikeout_కొలత);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_STRIKEOUT_స్థానభ్రంశం,            yStrikeoutPosition);

      ఒకవేళ (os2_ప్రధానసంఖ్య->సంస్కరణ >= 2)
      {
        hb_barrier ();
        స్వయంచాలక *పట్టిక = & const_cast<OS2_V2_తోక &> (os2_ప్రధానసంఖ్య->v2 ());
        HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_X_ఎత్తు,                   sx_ఎత్తు);
        HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_CAP_ఎత్తు,                 s_Cap_ఎత్తు);
      }

      గుర్తులేని avg_char_వెడల్పు = calc_avg_char_వెడల్పు (c->plan->hmtx_map);
      ఒకవేళ (!c->serializer->check_కేటాయించు (os2_ప్రధానసంఖ్య->x_Avg_Char_వెడల్పు, avg_char_వెడల్పు,
                                        HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
        return_trace (అబద్ధం);
    }
#అయితే_ముగింపు

    Triple *axis_range;
    ఒకవేళ (c->plan->user_axes_location.has (HB_ట్యాగ్ ('w','g','h','t'), &axis_range))
    {
      గుర్తులేని బరువు_class = static_cast<గుర్తులేని> (roundf (hb_clamp (axis_range->మధ్య, 1.0f, 1000.0f)));
      ఒకవేళ (os2_ప్రధానసంఖ్య->us_బరువు_Class != బరువు_class)
        os2_ప్రధానసంఖ్య->us_బరువు_Class = బరువు_class;
    }

    ఒకవేళ (c->plan->user_axes_location.has (HB_ట్యాగ్ ('w','d','t','h'), &axis_range))
    {
      గుర్తులేని వెడల్పు_class = static_cast<గుర్తులేని> (roundf (map_wdth_గమ్యం_widthclass (axis_range->మధ్య)));
      ఒకవేళ (os2_ప్రధానసంఖ్య->us_వెడల్పు_Class != వెడల్పు_class)
        os2_ప్రధానసంఖ్య->us_వెడల్పు_Class = వెడల్పు_class;
    }

    os2_ప్రధానసంఖ్య->us_మొదటి_Char_సూచీ = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_ప్రధానసంఖ్య->us_చివరి_Char_సూచీ  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    ఒకవేళ (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (నిజం);

    _update_unicode_ranges (&c->plan->unicodes, os2_ప్రధానసంఖ్య->ulUnicodeRange);

    return_trace (నిజం);
  }

  శూన్య _update_unicode_ranges (స్థిరరాశి hb_సమితి_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) స్థిరరాశి
  {
    HBUINT32 new_బిట్లు[4];
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < 4; i++)
      new_బిట్లు[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    కొరకు (స్వయంచాలక cp : *codepoints)
    {
      గుర్తులేని పూర్ణాంకం bit = hb_ot_os2_get_unicode_range_బిట్టు (cp);
      ఒకవేళ (bit < 128)
      {
	గుర్తులేని పూర్ణాంకం ఖండం = bit / 32;
	గుర్తులేని పూర్ణాంకం బిట్టు_లోపల_ఖండం = bit % 32;
	గుర్తులేని పూర్ణాంకం ముసుగు = 1 << బిట్టు_లోపల_ఖండం;
	new_బిట్లు[ఖండం] = new_బిట్లు[ఖండం] | ముసుగు;
      }
      ఒకవేళ (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	new_బిట్లు[1] = new_బిట్లు[1] | (1 << 25);
      }
    }

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & new_బిట్లు[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  గణన font_పుట_t
  {
    FONT_పుట_NONE		= 0,
    FONT_పుట_HEBREW		= 0xB100, /* Hebrew Windows 3.1 font page */
    FONT_పుట_SIMP_ARABIC	= 0xB200, /* Simplified Arabic Windows 3.1 font page */
    FONT_పుట_TRAD_ARABIC	= 0xB300, /* Traditional Arabic Windows 3.1 font page */
    FONT_పుట_OEM_ARABIC	= 0xB400, /* OEM Arabic Windows 3.1 font page */
    FONT_పుట_SIMP_FARSI	= 0xBA00, /* Simplified Farsi Windows 3.1 font page */
    FONT_పుట_TRAD_FARSI	= 0xBB00, /* Traditional Farsi Windows 3.1 font page */
    FONT_పుట_THAI		= 0xDE00  /* Thai Windows 3.1 font page */
  };
  font_పుట_t get_font_పుట () స్థిరరాశి
  { తిరిగి (font_పుట_t) (సంస్కరణ == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_కొలత () స్థిరరాశి
  {
    size_t ఫలితం = min_కొలత;
    ఒకవేళ (సంస్కరణ >= 1) ఫలితం = hb_unsigned_కూడు_saturate (ఫలితం, v1X.get_కొలత ());
    ఒకవేళ (సంస్కరణ >= 2) ఫలితం = hb_unsigned_కూడు_saturate (ఫలితం, v2X.get_కొలత ());
    ఒకవేళ (సంస్కరణ >= 5) ఫలితం = hb_unsigned_కూడు_saturate (ఫలితం, v5X.get_కొలత ());
    తిరిగి ఫలితం;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది))) return_trace (అబద్ధం);
    hb_barrier ();
    ఒకవేళ (unlikely (సంస్కరణ >= 1 && !v1X.sanitize (c))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (సంస్కరణ >= 2 && !v2X.sanitize (c))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (సంస్కరణ >= 5 && !v5X.sanitize (c))) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  బహిరంగ:
  HBUINT16	సంస్కరణ;
  HBINT16	x_Avg_Char_వెడల్పు;
  HBUINT16	us_బరువు_Class;
  HBUINT16	us_వెడల్పు_Class;
  HBUINT16	fsType;
  HBINT16	ySubscriptXSize;
  HBINT16	ySubscriptYSize;
  HBINT16	ySubscriptXOffset;
  HBINT16	ySubscriptYOffset;
  HBINT16	ySuperscriptXSize;
  HBINT16	ySuperscriptYSize;
  HBINT16	ySuperscriptXOffset;
  HBINT16	ySuperscriptYOffset;
  HBINT16	y_Strikeout_కొలత;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  ట్యాగ్		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_మొదటి_Char_సూచీ;
  HBUINT16	us_చివరి_Char_సూచీ;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_పంక్తి_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_తోక	v1X;
  OS2_V2_తోక	v2X;
  OS2_V5_తోక	v5X;
  బహిరంగ:
  DEFINE_కొలత_MIN (78);
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_OS2_TABLE_HH */
