/*
 * Copyright © 2013  Google, Inc.
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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#شامل "hb-ot-shaper.hh"


/* Hangul shaper */


/* Same order as the feature array below */
شمار {
  _JMO,

  LJMO,
  VJMO,
  TJMO,

  پہلا_HANGUL_FEATURE = LJMO,
  HANGUL_FEATURE_گنتی = TJMO + 1
};

ساکن ثابت hb_لیبل_t hangul_features[HANGUL_FEATURE_گنتی] =
{
  HB_لیبل_NONE,
  HB_لیبل('l','j','m','o'),
  HB_لیبل('v','j','m','o'),
  HB_لیبل('t','j','m','o')
};

ساکن خالی
collect_features_hangul (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  برائے (غیرعلامتی صحیح i = پہلا_HANGUL_FEATURE; i < HANGUL_FEATURE_گنتی; i++)
    map->جوڑو_feature (hangul_features[i]);
}

ساکن خالی
override_features_hangul (hb_ot_shape_planner_t *plan)
{
  /* Uniscribe does not apply 'calt' for Hangul, and certain fonts
   * (Noto Sans CJK, Source Han Sans, etc) apply all of jamo lookups
   * in calt, which is not desirable.
   *
   * Rather than disabling 'calt' for the entire run, which also turns
   * it off for any other characters in the run, allocate a mask for it
   * and clear that mask on jamo only (in setup_masks_hangul).  That
   * keeps jamo out of reach of such lookups while letting the rest of
   * the run shape with 'calt' as usual.
   *
   * https://github.com/harfbuzz/harfbuzz/discussions/4853 */
  plan->map.جوڑو_feature (HB_لیبل('c','a','l','t'));
}

ساخت hangul_shape_plan_t
{
  hb_نقاب_t نقاب_array[HANGUL_FEATURE_گنتی];
  hb_نقاب_t calt_نقاب;
};

ساکن خالی *
ڈیٹا_create_hangul (ثابت hb_ot_shape_plan_t *plan)
{
  hangul_shape_plan_t *hangul_plan = (hangul_shape_plan_t *) hb_calloc (1, حجم (hangul_shape_plan_t));
  اگر (unlikely (!hangul_plan))
    واپس خالی_اشاریہ;

  برائے (غیرعلامتی صحیح i = 0; i < HANGUL_FEATURE_گنتی; i++)
    hangul_plan->نقاب_array[i] = plan->map.get_1_نقاب (hangul_features[i]);

  hangul_plan->calt_نقاب = plan->map.get_1_نقاب (HB_لیبل('c','a','l','t'));

  واپس hangul_plan;
}

ساکن خالی
ڈیٹا_destroy_hangul (خالی *ڈیٹا)
{
  hb_free (ڈیٹا);
}

/* Constants for algorithmic hangul syllable [de]composition. */
#تعریف_کرو LBase 0x1100u
#تعریف_کرو VBase 0x1161u
#تعریف_کرو TBase 0x11A7u
#تعریف_کرو LCount 19u
#تعریف_کرو VCount 21u
#تعریف_کرو TCount 28u
#تعریف_کرو SBase 0xAC00u
#تعریف_کرو NCount (VCount * TCount)
#تعریف_کرو SCount (LCount * NCount)

#تعریف_کرو isCombiningL(u) (hb_اندر_range<hb_codepoint_t> ((u), LBase, LBase+LCount-1))
#تعریف_کرو isCombiningV(u) (hb_اندر_range<hb_codepoint_t> ((u), VBase, VBase+VCount-1))
#تعریف_کرو isCombiningT(u) (hb_اندر_range<hb_codepoint_t> ((u), TBase+1, TBase+TCount-1))
#تعریف_کرو isCombinedS(u) (hb_اندر_range<hb_codepoint_t> ((u), SBase, SBase+SCount-1))

#تعریف_کرو isL(u) (hb_اندر_ranges<hb_codepoint_t> ((u), 0x1100u, 0x115Fu, 0xA960u, 0xA97Cu))
#تعریف_کرو isV(u) (hb_اندر_ranges<hb_codepoint_t> ((u), 0x1160u, 0x11A7u, 0xD7B0u, 0xD7C6u))
#تعریف_کرو isT(u) (hb_اندر_ranges<hb_codepoint_t> ((u), 0x11A8u, 0x11FFu, 0xD7CBu, 0xD7FBu))

#تعریف_کرو isHangulTone(u) (hb_اندر_range<hb_codepoint_t> ((u), 0x302Eu, 0x302Fu))

/* buffer var allocations */
#تعریف_کرو hangul_shaping_feature() ot_shaper_var_u8_auxiliary() /* hangul jamo shaping feature */

ساکن منطقی
is_zero_چوڑائی_char (hb_font_t *font,
		    hb_codepoint_t unicode)
{
  hb_codepoint_t glyph;
  واپس hb_font_get_glyph (font, unicode, 0, &glyph) && hb_font_get_glyph_h_advance (font, glyph) == 0;
}

ساکن خالی
preprocess_متن_hangul (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
			hb_بفر_t              *بفر,
			hb_font_t                *font)
{
  HB_بفر_ALLOCATE_VAR (بفر, hangul_shaping_feature);

  /* Hangul syllables come in two shapes: LV, and LVT.  Of those:
   *
   *   - LV can be precomposed, or decomposed.  Lets call those
   *     <LV> and <L,V>,
   *   - LVT can be fully precomposed, partially precomposed, or
   *     fully decomposed.  Ie. <LVT>, <LV,T>, or <L,V,T>.
   *
   * The composition / decomposition is mechanical.  However, not
   * all <L,V> sequences compose, and not all <LV,T> sequences
   * compose.
   *
   * Here are the specifics:
   *
   *   - <L>: U+1100..115F, U+A960..A97F
   *   - <V>: U+1160..11A7, U+D7B0..D7C7
   *   - <T>: U+11A8..11FF, U+D7CB..D7FB
   *
   *   - Only the <L,V> sequences for some of the U+11xx ranges combine.
   *   - Only <LV,T> sequences for some of the Ts in U+11xx range combine.
   *
   * Here is what we want to accomplish in this shaper:
   *
   *   - If the whole syllable can be precomposed, do that,
   *   - Otherwise, fully decompose and apply ljmo/vjmo/tjmo features.
   *   - If a valid syllable is followed by a Hangul tone mark, reorder the tone
   *     mark to precede the whole syllable - unless it is a zero-width glyph, in
   *     which case we leave it untouched, assuming it's designed to overstrike.
   *
   * That is, of the different possible syllables:
   *
   *   <L>
   *   <L,V>
   *   <L,V,T>
   *   <LV>
   *   <LVT>
   *   <LV, T>
   *
   * - <L> needs no work.
   *
   * - <LV> and <LVT> can stay the way they are if the font supports them, otherwise we
   *   should fully decompose them if font supports.
   *
   * - <L,V> and <L,V,T> we should compose if the whole thing can be composed.
   *
   * - <LV,T> we should compose if the whole thing can be composed, otherwise we should
   *   decompose.
   */

  بفر->clear_output ();
  غیرعلامتی صحیح آغاز = 0, انتہا = 0; /* Extent of most recently seen syllable;
				    * valid only if start < end
				    */
  غیرعلامتی صحیح گنتی = بفر->len;

  برائے (بفر->idx = 0; بفر->idx < گنتی && بفر->successful;)
  {
    hb_codepoint_t u = بفر->cur().codepoint;

    اگر (isHangulTone (u))
    {
      /*
       * We could cache the width of the tone marks and the existence of dotted-circle,
       * but the use of the Hangul tone mark characters seems to be rare enough that
       * I didn't bother for now.
       */
      اگر (آغاز < انتہا && انتہا == بفر->برآمد_len)
      {
	/* Tone mark follows a valid syllable; move it in front, unless it's zero width. */
	بفر->unsafe_مقصد_break_from_outbuffer (آغاز, بفر->idx);
	اگر (unlikely (!بفر->اگلا_glyph ())) توڑو;
	اگر (!is_zero_چوڑائی_char (font, u))
	{
	  بفر->ملاؤ_برآمد_clusters (آغاز, انتہا + 1);
	  hb_glyph_info_t *info = بفر->برآمد_info;
	  hb_glyph_info_t tone = info[انتہا];
	  memmove (&info[آغاز + 1], &info[آغاز], (انتہا - آغاز) * حجم (hb_glyph_info_t));
	  info[آغاز] = tone;
	}
      }
      ورنہ
      {
	/* No valid syllable as base for tone mark; try to insert dotted circle. */
	اگر (!(بفر->flags & HB_بفر_FLAG_DO_NOT_داخل_کرو_DOTTED_CIRCLE) &&
	    font->has_glyph (0x25CCu))
	{
	  hb_codepoint_t chars[2];
	  اگر (!is_zero_چوڑائی_char (font, u))
	  {
	    chars[0] = u;
	    chars[1] = 0x25CCu;
	  } ورنہ
	  {
	    chars[0] = 0x25CCu;
	    chars[1] = u;
	  }
	  (خالی) بفر->replace_glyphs (1, 2, chars);
	}
	ورنہ
	{
	  /* No dotted circle available in the font; just leave tone mark untouched. */
	  (خالی) بفر->اگلا_glyph ();
	}
      }
      آغاز = انتہا = بفر->برآمد_len;
      جاری;
    }

    آغاز = بفر->برآمد_len; /* Remember current position as a potential syllable start;
			      * will only be used if we set end to a later position.
			      */

    اگر (isL (u) && بفر->idx + 1 < گنتی)
    {
      hb_codepoint_t l = u;
      hb_codepoint_t v = بفر->cur(+1).codepoint;
      اگر (isV (v))
      {
	/* Have <L,V> or <L,V,T>. */
	hb_codepoint_t t = 0;
	غیرعلامتی صحیح tindex = 0;
	اگر (بفر->idx + 2 < گنتی)
	{
	  t = بفر->cur(+2).codepoint;
	  اگر (isT (t))
	    tindex = t - TBase; /* Only used if isCombiningT (t); otherwise invalid. */
	  ورنہ
	    t = 0; /* The next character was not a trailing jamo. */
	}
	بفر->unsafe_مقصد_break (بفر->idx, بفر->idx + (t ? 3 : 2));

	/* We've got a syllable <L,V,T?>; see if it can potentially be composed. */
	اگر (isCombiningL (l) && isCombiningV (v) && (t == 0 || isCombiningT (t)))
	{
	  /* Try to compose; if this succeeds, end is set to start+1. */
	  hb_codepoint_t s = SBase + (l - LBase) * NCount + (v - VBase) * TCount + tindex;
	  اگر (font->has_glyph (s))
	  {
	    (خالی) بفر->replace_glyphs (t ? 3 : 2, 1, &s);
	    انتہا = آغاز + 1;
	    جاری;
	  }
	}

	/* We didn't compose, either because it's an Old Hangul syllable without a
	 * precomposed character in Unicode, or because the font didn't support the
	 * necessary precomposed glyph.
	 * Set jamo features on the individual glyphs, and advance past them.
	 */
	بفر->cur().hangul_shaping_feature() = LJMO;
	(خالی) بفر->اگلا_glyph ();
	بفر->cur().hangul_shaping_feature() = VJMO;
	(خالی) بفر->اگلا_glyph ();
	اگر (t)
	{
	  بفر->cur().hangul_shaping_feature() = TJMO;
	  (خالی) بفر->اگلا_glyph ();
	  انتہا = آغاز + 3;
	}
	ورنہ
	  انتہا = آغاز + 2;
	اگر (unlikely (!بفر->successful))
	  توڑو;
	بفر->ملاؤ_برآمد_grapheme_clusters (آغاز, انتہا);
	جاری;
      }
    }

    ورنہ اگر (isCombinedS (u))
    {
      /* Have <LV>, <LVT>, or <LV,T> */
      hb_codepoint_t s = u;
      منطقی has_glyph = font->has_glyph (s);
      غیرعلامتی صحیح lindex = (s - SBase) / NCount;
      غیرعلامتی صحیح nindex = (s - SBase) % NCount;
      غیرعلامتی صحیح vindex = nindex / TCount;
      غیرعلامتی صحیح tindex = nindex % TCount;

      اگر (!tindex &&
	  بفر->idx + 1 < گنتی &&
	  isCombiningT (بفر->cur(+1).codepoint))
      {
	/* <LV,T>, try to combine. */
	غیرعلامتی صحیح new_tindex = بفر->cur(+1).codepoint - TBase;
	hb_codepoint_t new_s = s + new_tindex;
	اگر (font->has_glyph (new_s))
	{
	  (خالی) بفر->replace_glyphs (2, 1, &new_s);
	  انتہا = آغاز + 1;
	  جاری;
	}
	ورنہ
	  بفر->unsafe_مقصد_break (بفر->idx, بفر->idx + 2); /* Mark unsafe between LV and T. */
      }

      /* Otherwise, decompose if font doesn't support <LV> or <LVT>,
       * or if having non-combining <LV,T>.  Note that we already handled
       * combining <LV,T> above. */
      اگر (!has_glyph ||
	  (!tindex &&
	   بفر->idx + 1 < گنتی &&
	   isT (بفر->cur(+1).codepoint)))
      {
	hb_codepoint_t decomposed[3] = {LBase + lindex,
					VBase + vindex,
					TBase + tindex};
	اگر (font->has_glyph (decomposed[0]) &&
	    font->has_glyph (decomposed[1]) &&
	    (!tindex || font->has_glyph (decomposed[2])))
	{
	  غیرعلامتی صحیح s_len = tindex ? 3 : 2;
	  (خالی) بفر->replace_glyphs (1, s_len, decomposed);

	  /* If we decomposed an LV because of a non-combining T following,
	   * we want to include this T in the syllable.
	   */
	  اگر (has_glyph && !tindex)
	  {
	    (خالی) بفر->اگلا_glyph ();
	    s_len++;
	  }
	  اگر (unlikely (!بفر->successful))
	    توڑو;

	  /* We decomposed S: apply jamo features to the individual glyphs
	   * that are now in buffer->out_info.
	   */
	  hb_glyph_info_t *info = بفر->برآمد_info;
	  انتہا = آغاز + s_len;

	  غیرعلامتی صحیح i = آغاز;
	  info[i++].hangul_shaping_feature() = LJMO;
	  info[i++].hangul_shaping_feature() = VJMO;
	  اگر (i < انتہا)
	    info[i++].hangul_shaping_feature() = TJMO;

	  بفر->ملاؤ_برآمد_grapheme_clusters (آغاز, انتہا);
	  جاری;
	}
	ورنہ اگر ((!tindex && بفر->idx + 1 < گنتی && isT (بفر->cur(+1).codepoint)))
	  بفر->unsafe_مقصد_break (بفر->idx, بفر->idx + 2); /* Mark unsafe between LV and T. */
      }

      اگر (has_glyph)
      {
	/* We didn't decompose the S, so just advance past it and fall through. */
	انتہا = آغاز + 1;
      }
    }

    /* Didn't find a recognizable syllable, so we leave end <= start;
     * this will prevent tone-mark reordering happening.
     */
    (خالی) بفر->اگلا_glyph ();
  }
  بفر->ہمآہنگ ();
}

ساکن خالی
تیاری_masks_hangul (ثابت hb_ot_shape_plan_t *plan,
		    hb_بفر_t              *بفر,
		    hb_font_t                *font HB_غیر_مستعمل)
{
  ثابت hangul_shape_plan_t *hangul_plan = (ثابت hangul_shape_plan_t *) plan->ڈیٹا;

  اگر (likely (hangul_plan))
  {
    غیرعلامتی صحیح گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++, info++)
    {
      info->نقاب |= hangul_plan->نقاب_array[info->hangul_shaping_feature()];

      /* Keep 'calt' away from jamo; see override_features_hangul(). */
      hb_codepoint_t u = info->codepoint;
      اگر (isL (u) || isV (u) || isT (u))
	info->نقاب &= ~hangul_plan->calt_نقاب;
    }
  }

  HB_بفر_DEALLOCATE_VAR (بفر, hangul_shaping_feature);
}


ثابت hb_ot_shaper_t _hb_ot_shaper_hangul =
{
  collect_features_hangul,
  override_features_hangul,
  ڈیٹا_create_hangul,
  ڈیٹا_destroy_hangul,
  preprocess_متن_hangul,
  خالی_اشاریہ, /* postprocess_glyphs */
  خالی_اشاریہ, /* decompose */
  خالی_اشاریہ, /* compose */
  تیاری_masks_hangul,
  خالی_اشاریہ, /* reorder_marks */
  HB_لیبل_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_NONE,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE,
  سچ, /* fallback_position */
};


#اگر_ختم
