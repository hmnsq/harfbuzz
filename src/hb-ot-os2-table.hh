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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_OS2_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_OS2_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-unicode-ranges.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-mvar-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"

/*
 * OS/2 and Windows Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/os2
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_OS2 HB_ਟੈਗ('O','S','/','2')


ਨਾਮਾਕਾਸ਼ OT {

ਬਣਤਰ OS2_V1_ਪੂਛ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT32	ul_ਕੋਡ_ਪੰਨਾ_Range1;
  HBUINT32	ul_ਕੋਡ_ਪੰਨਾ_Range2;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ OS2_V2_ਪੂਛ
{
  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ sx_ਉਚਾਈ || s_Cap_ਉਚਾਈ; }

  ਅਚਲ OS2_V2_ਪੂਛ * ਸੰਚਾਲਕ -> () ਅਚਲ { ਵਾਪਸ ਇਹ; }
  OS2_V2_ਪੂਛ * ਸੰਚਾਲਕ -> () { ਵਾਪਸ ਇਹ; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBINT16	sx_ਉਚਾਈ;
  HBINT16	s_Cap_ਉਚਾਈ;
  HBUINT16	usDefaultChar;
  HBUINT16	usBreakChar;
  HBUINT16	usMaxContext;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};

ਬਣਤਰ OS2_V5_ਪੂਛ
{
  ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ get_optical_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਹੇਠਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਉੱਤਲਾ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਹੇਠਲਾ_optical_ਮਾਪ = us_ਹੇਠਲਾ_Optical_ਬਿੰਦੂ_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉੱਤਲਾ_optical_ਮਾਪ = us_ਉੱਤਲਾ_Optical_ਬਿੰਦੂ_ਮਾਪ;

    /* Per https://docs.microsoft.com/en-us/typography/opentype/spec/os2#lps */
    ਜੇ (ਹੇਠਲਾ_optical_ਮਾਪ < ਉੱਤਲਾ_optical_ਮਾਪ &&
	ਹੇਠਲਾ_optical_ਮਾਪ >= 1 && ਹੇਠਲਾ_optical_ਮਾਪ <= 0xFFFE &&
	ਉੱਤਲਾ_optical_ਮਾਪ >= 2 && ਉੱਤਲਾ_optical_ਮਾਪ <= 0xFFFF)
    {
      *ਹੇਠਲਾ = ਹੇਠਲਾ_optical_ਮਾਪ;
      *ਉੱਤਲਾ = ਉੱਤਲਾ_optical_ਮਾਪ;
      ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT16	us_ਹੇਠਲਾ_Optical_ਬਿੰਦੂ_ਮਾਪ;
  HBUINT16	us_ਉੱਤਲਾ_Optical_ਬਿੰਦੂ_ਮਾਪ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ OS2
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_OS2;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ us_ਭਾਰ_Class || us_ਚੌੜਾਈ_Class || us_ਪਹਿਲਾ_Char_ਸੂਚਕਾਂਕ || us_ਆਖਰੀ_Char_ਸੂਚਕਾਂਕ; }

  ਅਚਲ OS2_V1_ਪੂਛ &v1 () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ >= 1 ? v1X : Null (OS2_V1_ਪੂਛ); }
  ਅਚਲ OS2_V2_ਪੂਛ &v2 () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ >= 2 ? v2X : Null (OS2_V2_ਪੂਛ); }
  ਅਚਲ OS2_V5_ਪੂਛ &v5 () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ >= 5 ? v5X : Null (OS2_V5_ਪੂਛ); }

  ਗਿਣਤੀ selection_flag_t {
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

  ਤਾਰਕਿਕ        is_italic () ਅਚਲ { ਵਾਪਸ fsSelection & ITALIC; }
  ਤਾਰਕਿਕ       is_oblique () ਅਚਲ { ਵਾਪਸ fsSelection & OBLIQUE; }
  ਤਾਰਕਿਕ use_typo_metrics () ਅਚਲ { ਵਾਪਸ fsSelection & USE_TYPO_METRICS; }

  ਗਿਣਤੀ ਚੌੜਾਈ_class_t {
    FWIDTH_ULTRA_CONDENSED	= 1, /* 50% */
    FWIDTH_EXTRA_CONDENSED	= 2, /* 62.5% */
    FWIDTH_CONDENSED		= 3, /* 75% */
    FWIDTH_SEMI_CONDENSED	= 4, /* 87.5% */
    FWIDTH_ਸਾਧਾਰਨ		= 5, /* 100% */
    FWIDTH_SEMI_EXPANDED	= 6, /* 112.5% */
    FWIDTH_EXPANDED		= 7, /* 125% */
    FWIDTH_EXTRA_EXPANDED	= 8, /* 150% */
    FWIDTH_ULTRA_EXPANDED	= 9  /* 200% */
  };

  ਦਸ਼ਮਲਵ get_ਚੌੜਾਈ () ਅਚਲ
  {
    ਚੋਣ (us_ਚੌੜਾਈ_Class) {
    ਹਾਲਤ FWIDTH_ULTRA_CONDENSED:ਵਾਪਸ 50.f;
    ਹਾਲਤ FWIDTH_EXTRA_CONDENSED:ਵਾਪਸ 62.5f;
    ਹਾਲਤ FWIDTH_CONDENSED:	ਵਾਪਸ 75.f;
    ਹਾਲਤ FWIDTH_SEMI_CONDENSED:	ਵਾਪਸ 87.5f;
    ਮੂਲ:
    ਹਾਲਤ FWIDTH_ਸਾਧਾਰਨ:		ਵਾਪਸ 100.f;
    ਹਾਲਤ FWIDTH_SEMI_EXPANDED:	ਵਾਪਸ 112.5f;
    ਹਾਲਤ FWIDTH_EXPANDED:	ਵਾਪਸ 125.f;
    ਹਾਲਤ FWIDTH_EXTRA_EXPANDED:	ਵਾਪਸ 150.f;
    ਹਾਲਤ FWIDTH_ULTRA_EXPANDED:	ਵਾਪਸ 200.f;
    }
  }

  ਦਸ਼ਮਲਵ map_wdth_ਮੰਜ਼ਿਲ_widthclass(ਦਸ਼ਮਲਵ ਚੌੜਾਈ) ਅਚਲ
  {
    ਜੇ (ਚੌੜਾਈ < 50) ਵਾਪਸ 1.0f;
    ਜੇ (ਚੌੜਾਈ > 200) ਵਾਪਸ 9.0f;

    ਦਸ਼ਮਲਵ ਅਨੁਪਾਤ = (ਚੌੜਾਈ - 50) / 12.5f;
    ਪੂਰਨ a = (ਪੂਰਨ) floorf (ਅਨੁਪਾਤ);
    ਪੂਰਨ b = (ਪੂਰਨ) ceilf (ਅਨੁਪਾਤ);

    /* follow this maping:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/os2#uswidthclass
     */
    ਜੇ (b <= 6) // 50-125
    {
      ਜੇ (a == b) ਵਾਪਸ a + 1.0f;
    }
    ਨਹੀਂਤਾਂ ਜੇ (b == 7) // no mapping for 137.5
    {
      a = 6;
      b = 8;
    }
    ਨਹੀਂਤਾਂ ਜੇ (b == 8)
    {
      ਜੇ (a == b) ਵਾਪਸ 8.0f; // 150
      a = 6;
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (a == b && a == 12) ਵਾਪਸ 9.0f; //200
      b = 12;
      a = 8;
    }

    ਦਸ਼ਮਲਵ va = 50 + a * 12.5f;
    ਦਸ਼ਮਲਵ vb = 50 + b * 12.5f;

    ਦਸ਼ਮਲਵ ret =  a + (ਚੌੜਾਈ - va) / (vb - va);
    ਜੇ (a <= 6) ret += 1.0f;
    ਵਾਪਸ ret;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ calc_avg_char_ਚੌੜਾਈ (ਅਚਲ hb_hashmap_t<hb_codepoint_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>>& hmtx_map)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਚੌੜਾਈ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : hmtx_map.ਮੁੱਲਾਂ_ref ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = _.ਪਹਿਲਾ;
      ਜੇ (ਚੌੜਾਈ)
      {
        ਕੁੱਲ_ਚੌੜਾਈ += ਚੌੜਾਈ;
        ਅੰਕ++;
      }
    }

    ਵਾਪਸ ਅੰਕ ? (ਬਿਨਾਨਿਸ਼ਾਨੀ) roundf ((ਦੁੱਗਣਾ) ਕੁੱਲ_ਚੌੜਾਈ / (ਦੁੱਗਣਾ) ਅੰਕ) : 0;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    OS2 *os2_ਅਭਾਜ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!os2_ਅਭਾਜ)) return_trace (ਝੂਠ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (c->plan->normalized_coords)
    {
      ਸਵੈਚਾਲਿਤ &MVAR = *c->plan->ਸਰੋਤ->ਸਾਰਣੀ.MVAR;
      ਸਵੈਚਾਲਿਤ *ਸਾਰਣੀ = os2_ਅਭਾਜ;

      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER,         sTypoAscender);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER,        sTypoDescender);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP,         s_Typo_ਸਤਰ_Gap);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CLIPPING_ASCENT,  usWinAscent);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CLIPPING_DESCENT, usWinDescent);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_X_ਮਾਪ,         ySubscriptXSize);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_Y_ਮਾਪ,         ySubscriptYSize);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_X_ਵਿੱਥ,       ySubscriptXOffset);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_Y_ਵਿੱਥ,       ySubscriptYOffset);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_X_ਮਾਪ,       ySuperscriptXSize);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_Y_ਮਾਪ,       ySuperscriptYSize);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_X_ਵਿੱਥ,     ySuperscriptXOffset);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_Y_ਵਿੱਥ,     ySuperscriptYOffset);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_STRIKEOUT_ਮਾਪ,              y_Strikeout_ਮਾਪ);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_STRIKEOUT_ਵਿੱਥ,            yStrikeoutPosition);

      ਜੇ (os2_ਅਭਾਜ->ਸੰਸਕਰਣ >= 2)
      {
        hb_barrier ();
        ਸਵੈਚਾਲਿਤ *ਸਾਰਣੀ = & const_cast<OS2_V2_ਪੂਛ &> (os2_ਅਭਾਜ->v2 ());
        HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_X_ਉਚਾਈ,                   sx_ਉਚਾਈ);
        HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_CAP_ਉਚਾਈ,                 s_Cap_ਉਚਾਈ);
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ avg_char_ਚੌੜਾਈ = calc_avg_char_ਚੌੜਾਈ (c->plan->hmtx_map);
      ਜੇ (!c->serializer->check_ਸੌਂਪੋ (os2_ਅਭਾਜ->x_Avg_Char_ਚੌੜਾਈ, avg_char_ਚੌੜਾਈ,
                                        HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
        return_trace (ਝੂਠ);
    }
#ਜੇ_ਅੰਤ

    Triple *axis_range;
    ਜੇ (c->plan->user_axes_location.has (HB_ਟੈਗ ('w','g','h','t'), &axis_range))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਭਾਰ_class = static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ> (roundf (hb_clamp (axis_range->ਵਿਚਕਾਰ, 1.0f, 1000.0f)));
      ਜੇ (os2_ਅਭਾਜ->us_ਭਾਰ_Class != ਭਾਰ_class)
        os2_ਅਭਾਜ->us_ਭਾਰ_Class = ਭਾਰ_class;
    }

    ਜੇ (c->plan->user_axes_location.has (HB_ਟੈਗ ('w','d','t','h'), &axis_range))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ_class = static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ> (roundf (map_wdth_ਮੰਜ਼ਿਲ_widthclass (axis_range->ਵਿਚਕਾਰ)));
      ਜੇ (os2_ਅਭਾਜ->us_ਚੌੜਾਈ_Class != ਚੌੜਾਈ_class)
        os2_ਅਭਾਜ->us_ਚੌੜਾਈ_Class = ਚੌੜਾਈ_class;
    }

    os2_ਅਭਾਜ->us_ਪਹਿਲਾ_Char_ਸੂਚਕਾਂਕ = hb_min (0xFFFFu, c->plan->os2_info.min_cmap_codepoint);
    os2_ਅਭਾਜ->us_ਆਖਰੀ_Char_ਸੂਚਕਾਂਕ  = hb_min (0xFFFFu, c->plan->os2_info.max_cmap_codepoint);

    ਜੇ (c->plan->flags & HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES)
      return_trace (ਸੱਚ);

    _update_unicode_ranges (&c->plan->unicodes, os2_ਅਭਾਜ->ulUnicodeRange);

    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ _update_unicode_ranges (ਅਚਲ hb_ਸੈੱਟ_t *codepoints,
			       HBUINT32 ulUnicodeRange[4]) ਅਚਲ
  {
    HBUINT32 new_ਬਿੱਟ[4];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < 4; i++)
      new_ਬਿੱਟ[i] = 0;

    /* This block doesn't show up in profiles. If it ever did,
     * we can rewrite it to iterate over OS/2 ranges and use
     * set iteration to check if the range matches. */
    ਲਈ (ਸਵੈਚਾਲਿਤ cp : *codepoints)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ bit = hb_ot_os2_get_unicode_range_ਦੋਆਧਾਰੀ_ਅੰਕ (cp);
      ਜੇ (bit < 128)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ = bit / 32;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_ਵਿੱਚ_ਖੰਡ = bit % 32;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = 1 << ਦੋਆਧਾਰੀ_ਅੰਕ_ਵਿੱਚ_ਖੰਡ;
	new_ਬਿੱਟ[ਖੰਡ] = new_ਬਿੱਟ[ਖੰਡ] | ਨਕਾਬ;
      }
      ਜੇ (cp >= 0x10000 && cp <= 0x110000)
      {
	/* the spec says that bit 57 ("Non Plane 0") implies that there's
	   at least one codepoint beyond the BMP; so I also include all
	   the non-BMP codepoints here */
	new_ਬਿੱਟ[1] = new_ਬਿੱਟ[1] | (1 << 25);
      }
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < 4; i++)
      ulUnicodeRange[i] = ulUnicodeRange[i] & new_ਬਿੱਟ[i]; // set bits only if set in the original
  }

  /* https://github.com/Microsoft/Font-Validator/blob/520aaae/OTFontFileVal/val_OS2.cs#L644-L681
   * https://docs.microsoft.com/en-us/typography/legacy/legacy_arabic_fonts */
  ਗਿਣਤੀ font_ਪੰਨਾ_t
  {
    FONT_ਪੰਨਾ_NONE		= 0,
    FONT_ਪੰਨਾ_HEBREW		= 0xB100, /* Hebrew Windows 3.1 font page */
    FONT_ਪੰਨਾ_SIMP_ARABIC	= 0xB200, /* Simplified Arabic Windows 3.1 font page */
    FONT_ਪੰਨਾ_TRAD_ARABIC	= 0xB300, /* Traditional Arabic Windows 3.1 font page */
    FONT_ਪੰਨਾ_OEM_ARABIC	= 0xB400, /* OEM Arabic Windows 3.1 font page */
    FONT_ਪੰਨਾ_SIMP_FARSI	= 0xBA00, /* Simplified Farsi Windows 3.1 font page */
    FONT_ਪੰਨਾ_TRAD_FARSI	= 0xBB00, /* Traditional Farsi Windows 3.1 font page */
    FONT_ਪੰਨਾ_THAI		= 0xDE00  /* Thai Windows 3.1 font page */
  };
  font_ਪੰਨਾ_t get_font_ਪੰਨਾ () ਅਚਲ
  { ਵਾਪਸ (font_ਪੰਨਾ_t) (ਸੰਸਕਰਣ == 0 ? fsSelection & 0xFF00 : 0); }

  size_t get_ਮਾਪ () ਅਚਲ
  {
    size_t ਨਤੀਜਾ = min_ਮਾਪ;
    ਜੇ (ਸੰਸਕਰਣ >= 1) ਨਤੀਜਾ = hb_unsigned_ਜੋੜੋ_saturate (ਨਤੀਜਾ, v1X.get_ਮਾਪ ());
    ਜੇ (ਸੰਸਕਰਣ >= 2) ਨਤੀਜਾ = hb_unsigned_ਜੋੜੋ_saturate (ਨਤੀਜਾ, v2X.get_ਮਾਪ ());
    ਜੇ (ਸੰਸਕਰਣ >= 5) ਨਤੀਜਾ = hb_unsigned_ਜੋੜੋ_saturate (ਨਤੀਜਾ, v5X.get_ਮਾਪ ());
    ਵਾਪਸ ਨਤੀਜਾ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਜੇ (unlikely (ਸੰਸਕਰਣ >= 1 && !v1X.sanitize (c))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (ਸੰਸਕਰਣ >= 2 && !v2X.sanitize (c))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (ਸੰਸਕਰਣ >= 5 && !v5X.sanitize (c))) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਜਨਤਕ:
  HBUINT16	ਸੰਸਕਰਣ;
  HBINT16	x_Avg_Char_ਚੌੜਾਈ;
  HBUINT16	us_ਭਾਰ_Class;
  HBUINT16	us_ਚੌੜਾਈ_Class;
  HBUINT16	fsType;
  HBINT16	ySubscriptXSize;
  HBINT16	ySubscriptYSize;
  HBINT16	ySubscriptXOffset;
  HBINT16	ySubscriptYOffset;
  HBINT16	ySuperscriptXSize;
  HBINT16	ySuperscriptYSize;
  HBINT16	ySuperscriptXOffset;
  HBINT16	ySuperscriptYOffset;
  HBINT16	y_Strikeout_ਮਾਪ;
  HBINT16	yStrikeoutPosition;
  HBINT16	sFamilyClass;
  HBUINT8	panose[10];
  HBUINT32	ulUnicodeRange[4];
  ਟੈਗ		achVendID;
  HBUINT16	fsSelection;
  HBUINT16	us_ਪਹਿਲਾ_Char_ਸੂਚਕਾਂਕ;
  HBUINT16	us_ਆਖਰੀ_Char_ਸੂਚਕਾਂਕ;
  HBINT16	sTypoAscender;
  HBINT16	sTypoDescender;
  HBINT16	s_Typo_ਸਤਰ_Gap;
  HBUINT16	usWinAscent;
  HBUINT16	usWinDescent;
  OS2_V1_ਪੂਛ	v1X;
  OS2_V2_ਪੂਛ	v2X;
  OS2_V5_ਪੂਛ	v5X;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (78);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_OS2_TABLE_HH */
