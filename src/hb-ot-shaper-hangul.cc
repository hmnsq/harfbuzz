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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper.hh"


/* Hangul shaper */


/* Same order as the feature array below */
ਗਿਣਤੀ {
  _JMO,

  LJMO,
  VJMO,
  TJMO,

  ਪਹਿਲਾ_HANGUL_FEATURE = LJMO,
  HANGUL_FEATURE_ਗਿਣਤੀ = TJMO + 1
};

ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t hangul_features[HANGUL_FEATURE_ਗਿਣਤੀ] =
{
  HB_ਟੈਗ_NONE,
  HB_ਟੈਗ('l','j','m','o'),
  HB_ਟੈਗ('v','j','m','o'),
  HB_ਟੈਗ('t','j','m','o')
};

ਸਥਿਰ ਖਾਲੀ
collect_features_hangul (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਪਹਿਲਾ_HANGUL_FEATURE; i < HANGUL_FEATURE_ਗਿਣਤੀ; i++)
    map->ਜੋੜੋ_feature (hangul_features[i]);
}

ਸਥਿਰ ਖਾਲੀ
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
  plan->map.ਜੋੜੋ_feature (HB_ਟੈਗ('c','a','l','t'));
}

ਬਣਤਰ hangul_shape_plan_t
{
  hb_ਨਕਾਬ_t ਨਕਾਬ_array[HANGUL_FEATURE_ਗਿਣਤੀ];
  hb_ਨਕਾਬ_t calt_ਨਕਾਬ;
};

ਸਥਿਰ ਖਾਲੀ *
ਅੰਕੜੇ_create_hangul (ਅਚਲ hb_ot_shape_plan_t *plan)
{
  hangul_shape_plan_t *hangul_plan = (hangul_shape_plan_t *) hb_calloc (1, ਆਕਾਰ (hangul_shape_plan_t));
  ਜੇ (unlikely (!hangul_plan))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < HANGUL_FEATURE_ਗਿਣਤੀ; i++)
    hangul_plan->ਨਕਾਬ_array[i] = plan->map.get_1_ਨਕਾਬ (hangul_features[i]);

  hangul_plan->calt_ਨਕਾਬ = plan->map.get_1_ਨਕਾਬ (HB_ਟੈਗ('c','a','l','t'));

  ਵਾਪਸ hangul_plan;
}

ਸਥਿਰ ਖਾਲੀ
ਅੰਕੜੇ_destroy_hangul (ਖਾਲੀ *ਅੰਕੜੇ)
{
  hb_free (ਅੰਕੜੇ);
}

/* Constants for algorithmic hangul syllable [de]composition. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBase 0x1100u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VBase 0x1161u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TBase 0x11A7u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LCount 19u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VCount 21u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TCount 28u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SBase 0xAC00u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NCount (VCount * TCount)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCount (LCount * NCount)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isCombiningL(u) (hb_ਵਿੱਚ_range<hb_codepoint_t> ((u), LBase, LBase+LCount-1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isCombiningV(u) (hb_ਵਿੱਚ_range<hb_codepoint_t> ((u), VBase, VBase+VCount-1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isCombiningT(u) (hb_ਵਿੱਚ_range<hb_codepoint_t> ((u), TBase+1, TBase+TCount-1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isCombinedS(u) (hb_ਵਿੱਚ_range<hb_codepoint_t> ((u), SBase, SBase+SCount-1))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isL(u) (hb_ਵਿੱਚ_ranges<hb_codepoint_t> ((u), 0x1100u, 0x115Fu, 0xA960u, 0xA97Cu))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isV(u) (hb_ਵਿੱਚ_ranges<hb_codepoint_t> ((u), 0x1160u, 0x11A7u, 0xD7B0u, 0xD7C6u))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isT(u) (hb_ਵਿੱਚ_ranges<hb_codepoint_t> ((u), 0x11A8u, 0x11FFu, 0xD7CBu, 0xD7FBu))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ isHangulTone(u) (hb_ਵਿੱਚ_range<hb_codepoint_t> ((u), 0x302Eu, 0x302Fu))

/* buffer var allocations */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hangul_shaping_feature() ot_shaper_var_u8_auxiliary() /* hangul jamo shaping feature */

ਸਥਿਰ ਤਾਰਕਿਕ
is_zero_ਚੌੜਾਈ_char (hb_font_t *font,
		    hb_codepoint_t unicode)
{
  hb_codepoint_t glyph;
  ਵਾਪਸ hb_font_get_glyph (font, unicode, 0, &glyph) && hb_font_get_glyph_h_advance (font, glyph) == 0;
}

ਸਥਿਰ ਖਾਲੀ
preprocess_ਪਾਠ_hangul (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
			hb_ਬਫਰ_t              *ਬਫਰ,
			hb_font_t                *font)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, hangul_shaping_feature);

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

  ਬਫਰ->clear_output ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0, ਸਿਰਾ = 0; /* Extent of most recently seen syllable;
				    * valid only if start < end
				    */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;

  ਲਈ (ਬਫਰ->idx = 0; ਬਫਰ->idx < ਗਿਣਤੀ_2 && ਬਫਰ->successful;)
  {
    hb_codepoint_t u = ਬਫਰ->cur().codepoint;

    ਜੇ (isHangulTone (u))
    {
      /*
       * We could cache the width of the tone marks and the existence of dotted-circle,
       * but the use of the Hangul tone mark characters seems to be rare enough that
       * I didn't bother for now.
       */
      ਜੇ (ਸ਼ੁਰੂ < ਸਿਰਾ && ਸਿਰਾ == ਬਫਰ->ਬਾਹਰਲਾ_len)
      {
	/* Tone mark follows a valid syllable; move it in front, unless it's zero width. */
	ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break_from_outbuffer (ਸ਼ੁਰੂ, ਬਫਰ->idx);
	ਜੇ (unlikely (!ਬਫਰ->ਅਗਲਾ_glyph ())) ਰੋਕੋ;
	ਜੇ (!is_zero_ਚੌੜਾਈ_char (font, u))
	{
	  ਬਫਰ->ਮਿਲਾਓ_ਬਾਹਰਲਾ_clusters (ਸ਼ੁਰੂ, ਸਿਰਾ + 1);
	  hb_glyph_info_t *info = ਬਫਰ->ਬਾਹਰਲਾ_info;
	  hb_glyph_info_t tone = info[ਸਿਰਾ];
	  memmove (&info[ਸ਼ੁਰੂ + 1], &info[ਸ਼ੁਰੂ], (ਸਿਰਾ - ਸ਼ੁਰੂ) * ਆਕਾਰ (hb_glyph_info_t));
	  info[ਸ਼ੁਰੂ] = tone;
	}
      }
      ਨਹੀਂਤਾਂ
      {
	/* No valid syllable as base for tone mark; try to insert dotted circle. */
	ਜੇ (!(ਬਫਰ->flags & HB_ਬਫਰ_FLAG_DO_NOT_ਪਾਓ_DOTTED_CIRCLE) &&
	    font->has_glyph (0x25CCu))
	{
	  hb_codepoint_t chars[2];
	  ਜੇ (!is_zero_ਚੌੜਾਈ_char (font, u))
	  {
	    chars[0] = u;
	    chars[1] = 0x25CCu;
	  } ਨਹੀਂਤਾਂ
	  {
	    chars[0] = 0x25CCu;
	    chars[1] = u;
	  }
	  (ਖਾਲੀ) ਬਫਰ->replace_glyphs (1, 2, chars);
	}
	ਨਹੀਂਤਾਂ
	{
	  /* No dotted circle available in the font; just leave tone mark untouched. */
	  (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
	}
      }
      ਸ਼ੁਰੂ = ਸਿਰਾ = ਬਫਰ->ਬਾਹਰਲਾ_len;
      ਜਾਰੀ;
    }

    ਸ਼ੁਰੂ = ਬਫਰ->ਬਾਹਰਲਾ_len; /* Remember current position as a potential syllable start;
			      * will only be used if we set end to a later position.
			      */

    ਜੇ (isL (u) && ਬਫਰ->idx + 1 < ਗਿਣਤੀ_2)
    {
      hb_codepoint_t l = u;
      hb_codepoint_t v = ਬਫਰ->cur(+1).codepoint;
      ਜੇ (isV (v))
      {
	/* Have <L,V> or <L,V,T>. */
	hb_codepoint_t t = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tindex = 0;
	ਜੇ (ਬਫਰ->idx + 2 < ਗਿਣਤੀ_2)
	{
	  t = ਬਫਰ->cur(+2).codepoint;
	  ਜੇ (isT (t))
	    tindex = t - TBase; /* Only used if isCombiningT (t); otherwise invalid. */
	  ਨਹੀਂਤਾਂ
	    t = 0; /* The next character was not a trailing jamo. */
	}
	ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਬਫਰ->idx, ਬਫਰ->idx + (t ? 3 : 2));

	/* We've got a syllable <L,V,T?>; see if it can potentially be composed. */
	ਜੇ (isCombiningL (l) && isCombiningV (v) && (t == 0 || isCombiningT (t)))
	{
	  /* Try to compose; if this succeeds, end is set to start+1. */
	  hb_codepoint_t s = SBase + (l - LBase) * NCount + (v - VBase) * TCount + tindex;
	  ਜੇ (font->has_glyph (s))
	  {
	    (ਖਾਲੀ) ਬਫਰ->replace_glyphs (t ? 3 : 2, 1, &s);
	    ਸਿਰਾ = ਸ਼ੁਰੂ + 1;
	    ਜਾਰੀ;
	  }
	}

	/* We didn't compose, either because it's an Old Hangul syllable without a
	 * precomposed character in Unicode, or because the font didn't support the
	 * necessary precomposed glyph.
	 * Set jamo features on the individual glyphs, and advance past them.
	 */
	ਬਫਰ->cur().hangul_shaping_feature() = LJMO;
	(ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
	ਬਫਰ->cur().hangul_shaping_feature() = VJMO;
	(ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
	ਜੇ (t)
	{
	  ਬਫਰ->cur().hangul_shaping_feature() = TJMO;
	  (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
	  ਸਿਰਾ = ਸ਼ੁਰੂ + 3;
	}
	ਨਹੀਂਤਾਂ
	  ਸਿਰਾ = ਸ਼ੁਰੂ + 2;
	ਜੇ (unlikely (!ਬਫਰ->successful))
	  ਰੋਕੋ;
	ਬਫਰ->ਮਿਲਾਓ_ਬਾਹਰਲਾ_grapheme_clusters (ਸ਼ੁਰੂ, ਸਿਰਾ);
	ਜਾਰੀ;
      }
    }

    ਨਹੀਂਤਾਂ ਜੇ (isCombinedS (u))
    {
      /* Have <LV>, <LVT>, or <LV,T> */
      hb_codepoint_t s = u;
      ਤਾਰਕਿਕ has_glyph = font->has_glyph (s);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lindex = (s - SBase) / NCount;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nindex = (s - SBase) % NCount;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ vindex = nindex / TCount;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tindex = nindex % TCount;

      ਜੇ (!tindex &&
	  ਬਫਰ->idx + 1 < ਗਿਣਤੀ_2 &&
	  isCombiningT (ਬਫਰ->cur(+1).codepoint))
      {
	/* <LV,T>, try to combine. */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_tindex = ਬਫਰ->cur(+1).codepoint - TBase;
	hb_codepoint_t new_s = s + new_tindex;
	ਜੇ (font->has_glyph (new_s))
	{
	  (ਖਾਲੀ) ਬਫਰ->replace_glyphs (2, 1, &new_s);
	  ਸਿਰਾ = ਸ਼ੁਰੂ + 1;
	  ਜਾਰੀ;
	}
	ਨਹੀਂਤਾਂ
	  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਬਫਰ->idx, ਬਫਰ->idx + 2); /* Mark unsafe between LV and T. */
      }

      /* Otherwise, decompose if font doesn't support <LV> or <LVT>,
       * or if having non-combining <LV,T>.  Note that we already handled
       * combining <LV,T> above. */
      ਜੇ (!has_glyph ||
	  (!tindex &&
	   ਬਫਰ->idx + 1 < ਗਿਣਤੀ_2 &&
	   isT (ਬਫਰ->cur(+1).codepoint)))
      {
	hb_codepoint_t decomposed[3] = {LBase + lindex,
					VBase + vindex,
					TBase + tindex};
	ਜੇ (font->has_glyph (decomposed[0]) &&
	    font->has_glyph (decomposed[1]) &&
	    (!tindex || font->has_glyph (decomposed[2])))
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ s_len = tindex ? 3 : 2;
	  (ਖਾਲੀ) ਬਫਰ->replace_glyphs (1, s_len, decomposed);

	  /* If we decomposed an LV because of a non-combining T following,
	   * we want to include this T in the syllable.
	   */
	  ਜੇ (has_glyph && !tindex)
	  {
	    (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
	    s_len++;
	  }
	  ਜੇ (unlikely (!ਬਫਰ->successful))
	    ਰੋਕੋ;

	  /* We decomposed S: apply jamo features to the individual glyphs
	   * that are now in buffer->out_info.
	   */
	  hb_glyph_info_t *info = ਬਫਰ->ਬਾਹਰਲਾ_info;
	  ਸਿਰਾ = ਸ਼ੁਰੂ + s_len;

	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ;
	  info[i++].hangul_shaping_feature() = LJMO;
	  info[i++].hangul_shaping_feature() = VJMO;
	  ਜੇ (i < ਸਿਰਾ)
	    info[i++].hangul_shaping_feature() = TJMO;

	  ਬਫਰ->ਮਿਲਾਓ_ਬਾਹਰਲਾ_grapheme_clusters (ਸ਼ੁਰੂ, ਸਿਰਾ);
	  ਜਾਰੀ;
	}
	ਨਹੀਂਤਾਂ ਜੇ ((!tindex && ਬਫਰ->idx + 1 < ਗਿਣਤੀ_2 && isT (ਬਫਰ->cur(+1).codepoint)))
	  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਬਫਰ->idx, ਬਫਰ->idx + 2); /* Mark unsafe between LV and T. */
      }

      ਜੇ (has_glyph)
      {
	/* We didn't decompose the S, so just advance past it and fall through. */
	ਸਿਰਾ = ਸ਼ੁਰੂ + 1;
      }
    }

    /* Didn't find a recognizable syllable, so we leave end <= start;
     * this will prevent tone-mark reordering happening.
     */
    (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
  }
  ਬਫਰ->ਸਮਕਾਲੀ ();
}

ਸਥਿਰ ਖਾਲੀ
ਤਿਆਰੀ_masks_hangul (ਅਚਲ hb_ot_shape_plan_t *plan,
		    hb_ਬਫਰ_t              *ਬਫਰ,
		    hb_font_t                *font HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hangul_shape_plan_t *hangul_plan = (ਅਚਲ hangul_shape_plan_t *) plan->ਅੰਕੜੇ;

  ਜੇ (likely (hangul_plan))
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
    hb_glyph_info_t *info = ਬਫਰ->info;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++, info++)
    {
      info->ਨਕਾਬ |= hangul_plan->ਨਕਾਬ_array[info->hangul_shaping_feature()];

      /* Keep 'calt' away from jamo; see override_features_hangul(). */
      hb_codepoint_t u = info->codepoint;
      ਜੇ (isL (u) || isV (u) || isT (u))
	info->ਨਕਾਬ &= ~hangul_plan->calt_ਨਕਾਬ;
    }
  }

  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, hangul_shaping_feature);
}


ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_hangul =
{
  collect_features_hangul,
  override_features_hangul,
  ਅੰਕੜੇ_create_hangul,
  ਅੰਕੜੇ_destroy_hangul,
  preprocess_ਪਾਠ_hangul,
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  ਖਾਲੀ_ਸੂਚਕ, /* decompose */
  ਖਾਲੀ_ਸੂਚਕ, /* compose */
  ਤਿਆਰੀ_masks_hangul,
  ਖਾਲੀ_ਸੂਚਕ, /* reorder_marks */
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_NONE,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE,
  ਸੱਚ, /* fallback_position */
};


#ਜੇ_ਅੰਤ
