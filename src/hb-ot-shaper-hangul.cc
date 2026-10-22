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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#समावेश "hb-ot-shaper.hh"


/* Hangul shaper */


/* Same order as the feature array below */
क्रमागत {
  _JMO,

  LJMO,
  VJMO,
  TJMO,

  FIRST_HANGUL_FEATURE = LJMO,
  HANGUL_FEATURE_गिनती = TJMO + 1
};

अटल स्थिर hb_tag_t hangul_features[HANGUL_FEATURE_गिनती] =
{
  HB_TAG_NONE,
  HB_TAG('l','j','m','o'),
  HB_TAG('v','j','m','o'),
  HB_TAG('t','j','m','o')
};

अटल व्योम
collect_features_hangul (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  क्रम (अचिन्हित पूर्णांक i = FIRST_HANGUL_FEATURE; i < HANGUL_FEATURE_गिनती; i++)
    map->add_feature (hangul_features[i]);
}

अटल व्योम
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
  plan->map.add_feature (HB_TAG('c','a','l','t'));
}

काष्ठा hangul_shape_plan_t
{
  hb_mask_t mask_array[HANGUL_FEATURE_गिनती];
  hb_mask_t calt_mask;
};

अटल व्योम *
तथ्य_create_hangul (स्थिर hb_ot_shape_plan_t *plan)
{
  hangul_shape_plan_t *hangul_plan = (hangul_shape_plan_t *) hb_calloc (1, माप (hangul_shape_plan_t));
  अगर (unlikely (!hangul_plan))
    वापस nullptr;

  क्रम (अचिन्हित पूर्णांक i = 0; i < HANGUL_FEATURE_गिनती; i++)
    hangul_plan->mask_array[i] = plan->map.get_1_mask (hangul_features[i]);

  hangul_plan->calt_mask = plan->map.get_1_mask (HB_TAG('c','a','l','t'));

  वापस hangul_plan;
}

अटल व्योम
तथ्य_destroy_hangul (व्योम *तथ्य)
{
  hb_free (तथ्य);
}

/* Constants for algorithmic hangul syllable [de]composition. */
#घोषणा LBase 0x1100u
#घोषणा VBase 0x1161u
#घोषणा TBase 0x11A7u
#घोषणा LCount 19u
#घोषणा VCount 21u
#घोषणा TCount 28u
#घोषणा SBase 0xAC00u
#घोषणा NCount (VCount * TCount)
#घोषणा SCount (LCount * NCount)

#घोषणा isCombiningL(u) (hb_in_range<hb_codepoint_t> ((u), LBase, LBase+LCount-1))
#घोषणा isCombiningV(u) (hb_in_range<hb_codepoint_t> ((u), VBase, VBase+VCount-1))
#घोषणा isCombiningT(u) (hb_in_range<hb_codepoint_t> ((u), TBase+1, TBase+TCount-1))
#घोषणा isCombinedS(u) (hb_in_range<hb_codepoint_t> ((u), SBase, SBase+SCount-1))

#घोषणा isL(u) (hb_in_ranges<hb_codepoint_t> ((u), 0x1100u, 0x115Fu, 0xA960u, 0xA97Cu))
#घोषणा isV(u) (hb_in_ranges<hb_codepoint_t> ((u), 0x1160u, 0x11A7u, 0xD7B0u, 0xD7C6u))
#घोषणा isT(u) (hb_in_ranges<hb_codepoint_t> ((u), 0x11A8u, 0x11FFu, 0xD7CBu, 0xD7FBu))

#घोषणा isHangulTone(u) (hb_in_range<hb_codepoint_t> ((u), 0x302Eu, 0x302Fu))

/* buffer var allocations */
#घोषणा hangul_shaping_feature() ot_shaper_var_u8_auxiliary() /* hangul jamo shaping feature */

अटल bool
is_zero_width_char (hb_font_t *font,
		    hb_codepoint_t unicode)
{
  hb_codepoint_t glyph;
  वापस hb_font_get_glyph (font, unicode, 0, &glyph) && hb_font_get_glyph_h_advance (font, glyph) == 0;
}

अटल व्योम
preprocess_text_hangul (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
			hb_बफर_t              *बफर,
			hb_font_t                *font)
{
  HB_बफर_ALLOCATE_VAR (बफर, hangul_shaping_feature);

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

  बफर->clear_output ();
  अचिन्हित पूर्णांक start = 0, end = 0; /* Extent of most recently seen syllable;
				    * valid only if start < end
				    */
  अचिन्हित पूर्णांक गिनती = बफर->len;

  क्रम (बफर->idx = 0; बफर->idx < गिनती && बफर->successful;)
  {
    hb_codepoint_t u = बफर->cur().codepoint;

    अगर (isHangulTone (u))
    {
      /*
       * We could cache the width of the tone marks and the existence of dotted-circle,
       * but the use of the Hangul tone mark characters seems to be rare enough that
       * I didn't bother for now.
       */
      अगर (start < end && end == बफर->out_len)
      {
	/* Tone mark follows a valid syllable; move it in front, unless it's zero width. */
	बफर->unsafe_to_break_from_outbuffer (start, बफर->idx);
	अगर (unlikely (!बफर->अग्र_glyph ())) अवरोध;
	अगर (!is_zero_width_char (font, u))
	{
	  बफर->merge_out_clusters (start, end + 1);
	  hb_glyph_info_t *info = बफर->out_info;
	  hb_glyph_info_t tone = info[end];
	  स_हटाओ (&info[start + 1], &info[start], (end - start) * माप (hb_glyph_info_t));
	  info[start] = tone;
	}
      }
      अन्यथा
      {
	/* No valid syllable as base for tone mark; try to insert dotted circle. */
	अगर (!(बफर->flags & HB_बफर_FLAG_DO_NOT_जोड़ो_DOTTED_CIRCLE) &&
	    font->has_glyph (0x25CCu))
	{
	  hb_codepoint_t chars[2];
	  अगर (!is_zero_width_char (font, u))
	  {
	    chars[0] = u;
	    chars[1] = 0x25CCu;
	  } अन्यथा
	  {
	    chars[0] = 0x25CCu;
	    chars[1] = u;
	  }
	  (व्योम) बफर->replace_glyphs (1, 2, chars);
	}
	अन्यथा
	{
	  /* No dotted circle available in the font; just leave tone mark untouched. */
	  (व्योम) बफर->अग्र_glyph ();
	}
      }
      start = end = बफर->out_len;
      जारी;
    }

    start = बफर->out_len; /* Remember current position as a potential syllable start;
			      * will only be used if we set end to a later position.
			      */

    अगर (isL (u) && बफर->idx + 1 < गिनती)
    {
      hb_codepoint_t l = u;
      hb_codepoint_t v = बफर->cur(+1).codepoint;
      अगर (isV (v))
      {
	/* Have <L,V> or <L,V,T>. */
	hb_codepoint_t t = 0;
	अचिन्हित पूर्णांक tindex = 0;
	अगर (बफर->idx + 2 < गिनती)
	{
	  t = बफर->cur(+2).codepoint;
	  अगर (isT (t))
	    tindex = t - TBase; /* Only used if isCombiningT (t); otherwise invalid. */
	  अन्यथा
	    t = 0; /* The next character was not a trailing jamo. */
	}
	बफर->unsafe_to_break (बफर->idx, बफर->idx + (t ? 3 : 2));

	/* We've got a syllable <L,V,T?>; see if it can potentially be composed. */
	अगर (isCombiningL (l) && isCombiningV (v) && (t == 0 || isCombiningT (t)))
	{
	  /* Try to compose; if this succeeds, end is set to start+1. */
	  hb_codepoint_t s = SBase + (l - LBase) * NCount + (v - VBase) * TCount + tindex;
	  अगर (font->has_glyph (s))
	  {
	    (व्योम) बफर->replace_glyphs (t ? 3 : 2, 1, &s);
	    end = start + 1;
	    जारी;
	  }
	}

	/* We didn't compose, either because it's an Old Hangul syllable without a
	 * precomposed character in Unicode, or because the font didn't support the
	 * necessary precomposed glyph.
	 * Set jamo features on the individual glyphs, and advance past them.
	 */
	बफर->cur().hangul_shaping_feature() = LJMO;
	(व्योम) बफर->अग्र_glyph ();
	बफर->cur().hangul_shaping_feature() = VJMO;
	(व्योम) बफर->अग्र_glyph ();
	अगर (t)
	{
	  बफर->cur().hangul_shaping_feature() = TJMO;
	  (व्योम) बफर->अग्र_glyph ();
	  end = start + 3;
	}
	अन्यथा
	  end = start + 2;
	अगर (unlikely (!बफर->successful))
	  अवरोध;
	बफर->merge_out_grapheme_clusters (start, end);
	जारी;
      }
    }

    अन्यथा अगर (isCombinedS (u))
    {
      /* Have <LV>, <LVT>, or <LV,T> */
      hb_codepoint_t s = u;
      bool has_glyph = font->has_glyph (s);
      अचिन्हित पूर्णांक lindex = (s - SBase) / NCount;
      अचिन्हित पूर्णांक nindex = (s - SBase) % NCount;
      अचिन्हित पूर्णांक vindex = nindex / TCount;
      अचिन्हित पूर्णांक tindex = nindex % TCount;

      अगर (!tindex &&
	  बफर->idx + 1 < गिनती &&
	  isCombiningT (बफर->cur(+1).codepoint))
      {
	/* <LV,T>, try to combine. */
	अचिन्हित पूर्णांक new_tindex = बफर->cur(+1).codepoint - TBase;
	hb_codepoint_t new_s = s + new_tindex;
	अगर (font->has_glyph (new_s))
	{
	  (व्योम) बफर->replace_glyphs (2, 1, &new_s);
	  end = start + 1;
	  जारी;
	}
	अन्यथा
	  बफर->unsafe_to_break (बफर->idx, बफर->idx + 2); /* Mark unsafe between LV and T. */
      }

      /* Otherwise, decompose if font doesn't support <LV> or <LVT>,
       * or if having non-combining <LV,T>.  Note that we already handled
       * combining <LV,T> above. */
      अगर (!has_glyph ||
	  (!tindex &&
	   बफर->idx + 1 < गिनती &&
	   isT (बफर->cur(+1).codepoint)))
      {
	hb_codepoint_t decomposed[3] = {LBase + lindex,
					VBase + vindex,
					TBase + tindex};
	अगर (font->has_glyph (decomposed[0]) &&
	    font->has_glyph (decomposed[1]) &&
	    (!tindex || font->has_glyph (decomposed[2])))
	{
	  अचिन्हित पूर्णांक s_len = tindex ? 3 : 2;
	  (व्योम) बफर->replace_glyphs (1, s_len, decomposed);

	  /* If we decomposed an LV because of a non-combining T following,
	   * we want to include this T in the syllable.
	   */
	  अगर (has_glyph && !tindex)
	  {
	    (व्योम) बफर->अग्र_glyph ();
	    s_len++;
	  }
	  अगर (unlikely (!बफर->successful))
	    अवरोध;

	  /* We decomposed S: apply jamo features to the individual glyphs
	   * that are now in buffer->out_info.
	   */
	  hb_glyph_info_t *info = बफर->out_info;
	  end = start + s_len;

	  अचिन्हित पूर्णांक i = start;
	  info[i++].hangul_shaping_feature() = LJMO;
	  info[i++].hangul_shaping_feature() = VJMO;
	  अगर (i < end)
	    info[i++].hangul_shaping_feature() = TJMO;

	  बफर->merge_out_grapheme_clusters (start, end);
	  जारी;
	}
	अन्यथा अगर ((!tindex && बफर->idx + 1 < गिनती && isT (बफर->cur(+1).codepoint)))
	  बफर->unsafe_to_break (बफर->idx, बफर->idx + 2); /* Mark unsafe between LV and T. */
      }

      अगर (has_glyph)
      {
	/* We didn't decompose the S, so just advance past it and fall through. */
	end = start + 1;
      }
    }

    /* Didn't find a recognizable syllable, so we leave end <= start;
     * this will prevent tone-mark reordering happening.
     */
    (व्योम) बफर->अग्र_glyph ();
  }
  बफर->sync ();
}

अटल व्योम
setup_masks_hangul (स्थिर hb_ot_shape_plan_t *plan,
		    hb_बफर_t              *बफर,
		    hb_font_t                *font HB_UNUSED)
{
  स्थिर hangul_shape_plan_t *hangul_plan = (स्थिर hangul_shape_plan_t *) plan->तथ्य;

  अगर (likely (hangul_plan))
  {
    अचिन्हित पूर्णांक गिनती = बफर->len;
    hb_glyph_info_t *info = बफर->info;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++, info++)
    {
      info->mask |= hangul_plan->mask_array[info->hangul_shaping_feature()];

      /* Keep 'calt' away from jamo; see override_features_hangul(). */
      hb_codepoint_t u = info->codepoint;
      अगर (isL (u) || isV (u) || isT (u))
	info->mask &= ~hangul_plan->calt_mask;
    }
  }

  HB_बफर_DEALLOCATE_VAR (बफर, hangul_shaping_feature);
}


स्थिर hb_ot_shaper_t _hb_ot_shaper_hangul =
{
  collect_features_hangul,
  override_features_hangul,
  तथ्य_create_hangul,
  तथ्य_destroy_hangul,
  preprocess_text_hangul,
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  nullptr, /* compose */
  setup_masks_hangul,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_NONE,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  true, /* fallback_position */
};


#पूर्ण_अगर
