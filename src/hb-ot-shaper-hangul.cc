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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shaper.hh"


/* Hangul shaper */


/* Same order as the feature array below */
గణన {
  _JMO,

  LJMO,
  VJMO,
  TJMO,

  మొదటి_HANGUL_FEATURE = LJMO,
  HANGUL_FEATURE_లెక్క = TJMO + 1
};

స్థిర స్థిరరాశి hb_ట్యాగ్_t hangul_features[HANGUL_FEATURE_లెక్క] =
{
  HB_ట్యాగ్_NONE,
  HB_ట్యాగ్('l','j','m','o'),
  HB_ట్యాగ్('v','j','m','o'),
  HB_ట్యాగ్('t','j','m','o')
};

స్థిర శూన్య
collect_features_hangul (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  కొరకు (గుర్తులేని పూర్ణాంకం i = మొదటి_HANGUL_FEATURE; i < HANGUL_FEATURE_లెక్క; i++)
    map->కూడు_feature (hangul_features[i]);
}

స్థిర శూన్య
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
  plan->map.కూడు_feature (HB_ట్యాగ్('c','a','l','t'));
}

నిర్మాణం hangul_shape_plan_t
{
  hb_ముసుగు_t ముసుగు_array[HANGUL_FEATURE_లెక్క];
  hb_ముసుగు_t calt_ముసుగు;
};

స్థిర శూన్య *
దత్తాంశం_create_hangul (స్థిరరాశి hb_ot_shape_plan_t *plan)
{
  hangul_shape_plan_t *hangul_plan = (hangul_shape_plan_t *) hb_calloc (1, పరిమాణం (hangul_shape_plan_t));
  ఒకవేళ (unlikely (!hangul_plan))
    తిరిగి శూన్యసూచిక;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < HANGUL_FEATURE_లెక్క; i++)
    hangul_plan->ముసుగు_array[i] = plan->map.get_1_ముసుగు (hangul_features[i]);

  hangul_plan->calt_ముసుగు = plan->map.get_1_ముసుగు (HB_ట్యాగ్('c','a','l','t'));

  తిరిగి hangul_plan;
}

స్థిర శూన్య
దత్తాంశం_destroy_hangul (శూన్య *దత్తాంశం)
{
  hb_free (దత్తాంశం);
}

/* Constants for algorithmic hangul syllable [de]composition. */
#నిర్వచించు LBase 0x1100u
#నిర్వచించు VBase 0x1161u
#నిర్వచించు TBase 0x11A7u
#నిర్వచించు LCount 19u
#నిర్వచించు VCount 21u
#నిర్వచించు TCount 28u
#నిర్వచించు SBase 0xAC00u
#నిర్వచించు NCount (VCount * TCount)
#నిర్వచించు SCount (LCount * NCount)

#నిర్వచించు isCombiningL(u) (hb_లోపల_range<hb_codepoint_t> ((u), LBase, LBase+LCount-1))
#నిర్వచించు isCombiningV(u) (hb_లోపల_range<hb_codepoint_t> ((u), VBase, VBase+VCount-1))
#నిర్వచించు isCombiningT(u) (hb_లోపల_range<hb_codepoint_t> ((u), TBase+1, TBase+TCount-1))
#నిర్వచించు isCombinedS(u) (hb_లోపల_range<hb_codepoint_t> ((u), SBase, SBase+SCount-1))

#నిర్వచించు isL(u) (hb_లోపల_ranges<hb_codepoint_t> ((u), 0x1100u, 0x115Fu, 0xA960u, 0xA97Cu))
#నిర్వచించు isV(u) (hb_లోపల_ranges<hb_codepoint_t> ((u), 0x1160u, 0x11A7u, 0xD7B0u, 0xD7C6u))
#నిర్వచించు isT(u) (hb_లోపల_ranges<hb_codepoint_t> ((u), 0x11A8u, 0x11FFu, 0xD7CBu, 0xD7FBu))

#నిర్వచించు isHangulTone(u) (hb_లోపల_range<hb_codepoint_t> ((u), 0x302Eu, 0x302Fu))

/* buffer var allocations */
#నిర్వచించు hangul_shaping_feature() ot_shaper_var_u8_auxiliary() /* hangul jamo shaping feature */

స్థిర తార్కిక
is_zero_వెడల్పు_char (hb_font_t *font,
		    hb_codepoint_t unicode)
{
  hb_codepoint_t glyph;
  తిరిగి hb_font_get_glyph (font, unicode, 0, &glyph) && hb_font_get_glyph_h_advance (font, glyph) == 0;
}

స్థిర శూన్య
preprocess_పాఠ్యం_hangul (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
			hb_బఫరు_t              *బఫరు,
			hb_font_t                *font)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, hangul_shaping_feature);

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

  బఫరు->clear_output ();
  గుర్తులేని పూర్ణాంకం ప్రారంభం = 0, చివర = 0; /* Extent of most recently seen syllable;
				    * valid only if start < end
				    */
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;

  కొరకు (బఫరు->idx = 0; బఫరు->idx < లెక్క && బఫరు->successful;)
  {
    hb_codepoint_t u = బఫరు->cur().codepoint;

    ఒకవేళ (isHangulTone (u))
    {
      /*
       * We could cache the width of the tone marks and the existence of dotted-circle,
       * but the use of the Hangul tone mark characters seems to be rare enough that
       * I didn't bother for now.
       */
      ఒకవేళ (ప్రారంభం < చివర && చివర == బఫరు->బహిర్గతం_len)
      {
	/* Tone mark follows a valid syllable; move it in front, unless it's zero width. */
	బఫరు->unsafe_గమ్యం_break_from_outbuffer (ప్రారంభం, బఫరు->idx);
	ఒకవేళ (unlikely (!బఫరు->తదుపరి_glyph ())) ఆపు;
	ఒకవేళ (!is_zero_వెడల్పు_char (font, u))
	{
	  బఫరు->విలీనం_బహిర్గతం_clusters (ప్రారంభం, చివర + 1);
	  hb_glyph_info_t *info = బఫరు->బహిర్గతం_info;
	  hb_glyph_info_t tone = info[చివర];
	  memmove (&info[ప్రారంభం + 1], &info[ప్రారంభం], (చివర - ప్రారంభం) * పరిమాణం (hb_glyph_info_t));
	  info[ప్రారంభం] = tone;
	}
      }
      లేకపోతే
      {
	/* No valid syllable as base for tone mark; try to insert dotted circle. */
	ఒకవేళ (!(బఫరు->flags & HB_బఫరు_FLAG_DO_NOT_చొప్పించు_DOTTED_CIRCLE) &&
	    font->has_glyph (0x25CCu))
	{
	  hb_codepoint_t chars[2];
	  ఒకవేళ (!is_zero_వెడల్పు_char (font, u))
	  {
	    chars[0] = u;
	    chars[1] = 0x25CCu;
	  } లేకపోతే
	  {
	    chars[0] = 0x25CCu;
	    chars[1] = u;
	  }
	  (శూన్య) బఫరు->replace_glyphs (1, 2, chars);
	}
	లేకపోతే
	{
	  /* No dotted circle available in the font; just leave tone mark untouched. */
	  (శూన్య) బఫరు->తదుపరి_glyph ();
	}
      }
      ప్రారంభం = చివర = బఫరు->బహిర్గతం_len;
      కొనసాగు;
    }

    ప్రారంభం = బఫరు->బహిర్గతం_len; /* Remember current position as a potential syllable start;
			      * will only be used if we set end to a later position.
			      */

    ఒకవేళ (isL (u) && బఫరు->idx + 1 < లెక్క)
    {
      hb_codepoint_t l = u;
      hb_codepoint_t v = బఫరు->cur(+1).codepoint;
      ఒకవేళ (isV (v))
      {
	/* Have <L,V> or <L,V,T>. */
	hb_codepoint_t t = 0;
	గుర్తులేని పూర్ణాంకం tindex = 0;
	ఒకవేళ (బఫరు->idx + 2 < లెక్క)
	{
	  t = బఫరు->cur(+2).codepoint;
	  ఒకవేళ (isT (t))
	    tindex = t - TBase; /* Only used if isCombiningT (t); otherwise invalid. */
	  లేకపోతే
	    t = 0; /* The next character was not a trailing jamo. */
	}
	బఫరు->unsafe_గమ్యం_break (బఫరు->idx, బఫరు->idx + (t ? 3 : 2));

	/* We've got a syllable <L,V,T?>; see if it can potentially be composed. */
	ఒకవేళ (isCombiningL (l) && isCombiningV (v) && (t == 0 || isCombiningT (t)))
	{
	  /* Try to compose; if this succeeds, end is set to start+1. */
	  hb_codepoint_t s = SBase + (l - LBase) * NCount + (v - VBase) * TCount + tindex;
	  ఒకవేళ (font->has_glyph (s))
	  {
	    (శూన్య) బఫరు->replace_glyphs (t ? 3 : 2, 1, &s);
	    చివర = ప్రారంభం + 1;
	    కొనసాగు;
	  }
	}

	/* We didn't compose, either because it's an Old Hangul syllable without a
	 * precomposed character in Unicode, or because the font didn't support the
	 * necessary precomposed glyph.
	 * Set jamo features on the individual glyphs, and advance past them.
	 */
	బఫరు->cur().hangul_shaping_feature() = LJMO;
	(శూన్య) బఫరు->తదుపరి_glyph ();
	బఫరు->cur().hangul_shaping_feature() = VJMO;
	(శూన్య) బఫరు->తదుపరి_glyph ();
	ఒకవేళ (t)
	{
	  బఫరు->cur().hangul_shaping_feature() = TJMO;
	  (శూన్య) బఫరు->తదుపరి_glyph ();
	  చివర = ప్రారంభం + 3;
	}
	లేకపోతే
	  చివర = ప్రారంభం + 2;
	ఒకవేళ (unlikely (!బఫరు->successful))
	  ఆపు;
	బఫరు->విలీనం_బహిర్గతం_grapheme_clusters (ప్రారంభం, చివర);
	కొనసాగు;
      }
    }

    లేకపోతే ఒకవేళ (isCombinedS (u))
    {
      /* Have <LV>, <LVT>, or <LV,T> */
      hb_codepoint_t s = u;
      తార్కిక has_glyph = font->has_glyph (s);
      గుర్తులేని పూర్ణాంకం lindex = (s - SBase) / NCount;
      గుర్తులేని పూర్ణాంకం nindex = (s - SBase) % NCount;
      గుర్తులేని పూర్ణాంకం vindex = nindex / TCount;
      గుర్తులేని పూర్ణాంకం tindex = nindex % TCount;

      ఒకవేళ (!tindex &&
	  బఫరు->idx + 1 < లెక్క &&
	  isCombiningT (బఫరు->cur(+1).codepoint))
      {
	/* <LV,T>, try to combine. */
	గుర్తులేని పూర్ణాంకం new_tindex = బఫరు->cur(+1).codepoint - TBase;
	hb_codepoint_t new_s = s + new_tindex;
	ఒకవేళ (font->has_glyph (new_s))
	{
	  (శూన్య) బఫరు->replace_glyphs (2, 1, &new_s);
	  చివర = ప్రారంభం + 1;
	  కొనసాగు;
	}
	లేకపోతే
	  బఫరు->unsafe_గమ్యం_break (బఫరు->idx, బఫరు->idx + 2); /* Mark unsafe between LV and T. */
      }

      /* Otherwise, decompose if font doesn't support <LV> or <LVT>,
       * or if having non-combining <LV,T>.  Note that we already handled
       * combining <LV,T> above. */
      ఒకవేళ (!has_glyph ||
	  (!tindex &&
	   బఫరు->idx + 1 < లెక్క &&
	   isT (బఫరు->cur(+1).codepoint)))
      {
	hb_codepoint_t decomposed[3] = {LBase + lindex,
					VBase + vindex,
					TBase + tindex};
	ఒకవేళ (font->has_glyph (decomposed[0]) &&
	    font->has_glyph (decomposed[1]) &&
	    (!tindex || font->has_glyph (decomposed[2])))
	{
	  గుర్తులేని పూర్ణాంకం s_len = tindex ? 3 : 2;
	  (శూన్య) బఫరు->replace_glyphs (1, s_len, decomposed);

	  /* If we decomposed an LV because of a non-combining T following,
	   * we want to include this T in the syllable.
	   */
	  ఒకవేళ (has_glyph && !tindex)
	  {
	    (శూన్య) బఫరు->తదుపరి_glyph ();
	    s_len++;
	  }
	  ఒకవేళ (unlikely (!బఫరు->successful))
	    ఆపు;

	  /* We decomposed S: apply jamo features to the individual glyphs
	   * that are now in buffer->out_info.
	   */
	  hb_glyph_info_t *info = బఫరు->బహిర్గతం_info;
	  చివర = ప్రారంభం + s_len;

	  గుర్తులేని పూర్ణాంకం i = ప్రారంభం;
	  info[i++].hangul_shaping_feature() = LJMO;
	  info[i++].hangul_shaping_feature() = VJMO;
	  ఒకవేళ (i < చివర)
	    info[i++].hangul_shaping_feature() = TJMO;

	  బఫరు->విలీనం_బహిర్గతం_grapheme_clusters (ప్రారంభం, చివర);
	  కొనసాగు;
	}
	లేకపోతే ఒకవేళ ((!tindex && బఫరు->idx + 1 < లెక్క && isT (బఫరు->cur(+1).codepoint)))
	  బఫరు->unsafe_గమ్యం_break (బఫరు->idx, బఫరు->idx + 2); /* Mark unsafe between LV and T. */
      }

      ఒకవేళ (has_glyph)
      {
	/* We didn't decompose the S, so just advance past it and fall through. */
	చివర = ప్రారంభం + 1;
      }
    }

    /* Didn't find a recognizable syllable, so we leave end <= start;
     * this will prevent tone-mark reordering happening.
     */
    (శూన్య) బఫరు->తదుపరి_glyph ();
  }
  బఫరు->సమకాలీకరణ ();
}

స్థిర శూన్య
అమరిక_masks_hangul (స్థిరరాశి hb_ot_shape_plan_t *plan,
		    hb_బఫరు_t              *బఫరు,
		    hb_font_t                *font HB_వాడని)
{
  స్థిరరాశి hangul_shape_plan_t *hangul_plan = (స్థిరరాశి hangul_shape_plan_t *) plan->దత్తాంశం;

  ఒకవేళ (likely (hangul_plan))
  {
    గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
    hb_glyph_info_t *info = బఫరు->info;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++, info++)
    {
      info->ముసుగు |= hangul_plan->ముసుగు_array[info->hangul_shaping_feature()];

      /* Keep 'calt' away from jamo; see override_features_hangul(). */
      hb_codepoint_t u = info->codepoint;
      ఒకవేళ (isL (u) || isV (u) || isT (u))
	info->ముసుగు &= ~hangul_plan->calt_ముసుగు;
    }
  }

  HB_బఫరు_DEALLOCATE_VAR (బఫరు, hangul_shaping_feature);
}


స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_hangul =
{
  collect_features_hangul,
  override_features_hangul,
  దత్తాంశం_create_hangul,
  దత్తాంశం_destroy_hangul,
  preprocess_పాఠ్యం_hangul,
  శూన్యసూచిక, /* postprocess_glyphs */
  శూన్యసూచిక, /* decompose */
  శూన్యసూచిక, /* compose */
  అమరిక_masks_hangul,
  శూన్యసూచిక, /* reorder_marks */
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_NONE,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_NONE,
  నిజం, /* fallback_position */
};


#అయితే_ముగింపు
