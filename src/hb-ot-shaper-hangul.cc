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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ಅನ್ತರ್ಭಾವ "hb-ot-shaper.hh"


/* Hangul shaper */


/* Same order as the feature array below */
ಗಣನಾ {
  _JMO,

  LJMO,
  VJMO,
  TJMO,

  FIRST_HANGUL_FEATURE = LJMO,
  HANGUL_FEATURE_ಎಣಿಕೆ = TJMO + 1
};

ಸ್ಥಿರ ಧ್ರುವ hb_tag_t hangul_features[HANGUL_FEATURE_ಎಣಿಕೆ] =
{
  HB_TAG_NONE,
  HB_TAG('l','j','m','o'),
  HB_TAG('v','j','m','o'),
  HB_TAG('t','j','m','o')
};

ಸ್ಥಿರ ಶೂನ್ಯ
collect_features_hangul (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = FIRST_HANGUL_FEATURE; i < HANGUL_FEATURE_ಎಣಿಕೆ; i++)
    map->add_feature (hangul_features[i]);
}

ಸ್ಥಿರ ಶೂನ್ಯ
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

ಸಂರಚನಾ hangul_shape_plan_t
{
  hb_mask_t mask_array[HANGUL_FEATURE_ಎಣಿಕೆ];
  hb_mask_t calt_mask;
};

ಸ್ಥಿರ ಶೂನ್ಯ *
data_create_hangul (ಧ್ರುವ hb_ot_shape_plan_t *plan)
{
  hangul_shape_plan_t *hangul_plan = (hangul_shape_plan_t *) hb_calloc (1, ಪರಿಮಾಣ (hangul_shape_plan_t));
  ಯದಿ (unlikely (!hangul_plan))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < HANGUL_FEATURE_ಎಣಿಕೆ; i++)
    hangul_plan->mask_array[i] = plan->map.get_1_mask (hangul_features[i]);

  hangul_plan->calt_mask = plan->map.get_1_mask (HB_TAG('c','a','l','t'));

  ಪ್ರತ್ಯಾಗಮ hangul_plan;
}

ಸ್ಥಿರ ಶೂನ್ಯ
data_destroy_hangul (ಶೂನ್ಯ *data)
{
  hb_free (data);
}

/* Constants for algorithmic hangul syllable [de]composition. */
#define LBase 0x1100u
#define VBase 0x1161u
#define TBase 0x11A7u
#define LCount 19u
#define VCount 21u
#define TCount 28u
#define SBase 0xAC00u
#define NCount (VCount * TCount)
#define SCount (LCount * NCount)

#define isCombiningL(u) (hb_in_range<hb_codepoint_t> ((u), LBase, LBase+LCount-1))
#define isCombiningV(u) (hb_in_range<hb_codepoint_t> ((u), VBase, VBase+VCount-1))
#define isCombiningT(u) (hb_in_range<hb_codepoint_t> ((u), TBase+1, TBase+TCount-1))
#define isCombinedS(u) (hb_in_range<hb_codepoint_t> ((u), SBase, SBase+SCount-1))

#define isL(u) (hb_in_ranges<hb_codepoint_t> ((u), 0x1100u, 0x115Fu, 0xA960u, 0xA97Cu))
#define isV(u) (hb_in_ranges<hb_codepoint_t> ((u), 0x1160u, 0x11A7u, 0xD7B0u, 0xD7C6u))
#define isT(u) (hb_in_ranges<hb_codepoint_t> ((u), 0x11A8u, 0x11FFu, 0xD7CBu, 0xD7FBu))

#define isHangulTone(u) (hb_in_range<hb_codepoint_t> ((u), 0x302Eu, 0x302Fu))

/* buffer var allocations */
#define hangul_shaping_feature() ot_shaper_var_u8_auxiliary() /* hangul jamo shaping feature */

ಸ್ಥಿರ ತಾರ್ಕಿಕ
is_zero_width_char (hb_font_t *font,
		    hb_codepoint_t unicode)
{
  hb_codepoint_t glyph;
  ಪ್ರತ್ಯಾಗಮ hb_font_get_glyph (font, unicode, 0, &glyph) && hb_font_get_glyph_h_advance (font, glyph) == 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
preprocess_text_hangul (ಧ್ರುವ hb_ot_shape_plan_t *plan HB_UNUSED,
			hb_buffer_t              *buffer,
			hb_font_t                *font)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, hangul_shaping_feature);

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

  buffer->clear_output ();
  unsigned ಪೂರ್ಣಾಙ್ಕ start = 0, end = 0; /* Extent of most recently seen syllable;
				    * valid only if start < end
				    */
  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buffer->len;

  ಪ್ರತಿ (buffer->idx = 0; buffer->idx < ಎಣಿಕೆ && buffer->successful;)
  {
    hb_codepoint_t u = buffer->cur().codepoint;

    ಯದಿ (isHangulTone (u))
    {
      /*
       * We could cache the width of the tone marks and the existence of dotted-circle,
       * but the use of the Hangul tone mark characters seems to be rare enough that
       * I didn't bother for now.
       */
      ಯದಿ (start < end && end == buffer->out_len)
      {
	/* Tone mark follows a valid syllable; move it in front, unless it's zero width. */
	buffer->unsafe_to_break_from_outbuffer (start, buffer->idx);
	ಯದಿ (unlikely (!buffer->next_glyph ())) ಭಙ್ಗ;
	ಯದಿ (!is_zero_width_char (font, u))
	{
	  buffer->merge_out_clusters (start, end + 1);
	  hb_glyph_info_t *info = buffer->out_info;
	  hb_glyph_info_t tone = info[end];
	  memmove (&info[start + 1], &info[start], (end - start) * ಪರಿಮಾಣ (hb_glyph_info_t));
	  info[start] = tone;
	}
      }
      ಅನ್ಯಥಾ
      {
	/* No valid syllable as base for tone mark; try to insert dotted circle. */
	ಯದಿ (!(buffer->flags & HB_BUFFER_FLAG_DO_NOT_ಸೇರಿಸು_DOTTED_CIRCLE) &&
	    font->has_glyph (0x25CCu))
	{
	  hb_codepoint_t chars[2];
	  ಯದಿ (!is_zero_width_char (font, u))
	  {
	    chars[0] = u;
	    chars[1] = 0x25CCu;
	  } ಅನ್ಯಥಾ
	  {
	    chars[0] = 0x25CCu;
	    chars[1] = u;
	  }
	  (ಶೂನ್ಯ) buffer->replace_glyphs (1, 2, chars);
	}
	ಅನ್ಯಥಾ
	{
	  /* No dotted circle available in the font; just leave tone mark untouched. */
	  (ಶೂನ್ಯ) buffer->next_glyph ();
	}
      }
      start = end = buffer->out_len;
      ಅನುವರ್ತ;
    }

    start = buffer->out_len; /* Remember current position as a potential syllable start;
			      * will only be used if we set end to a later position.
			      */

    ಯದಿ (isL (u) && buffer->idx + 1 < ಎಣಿಕೆ)
    {
      hb_codepoint_t l = u;
      hb_codepoint_t v = buffer->cur(+1).codepoint;
      ಯದಿ (isV (v))
      {
	/* Have <L,V> or <L,V,T>. */
	hb_codepoint_t t = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ tindex = 0;
	ಯದಿ (buffer->idx + 2 < ಎಣಿಕೆ)
	{
	  t = buffer->cur(+2).codepoint;
	  ಯದಿ (isT (t))
	    tindex = t - TBase; /* Only used if isCombiningT (t); otherwise invalid. */
	  ಅನ್ಯಥಾ
	    t = 0; /* The next character was not a trailing jamo. */
	}
	buffer->unsafe_to_break (buffer->idx, buffer->idx + (t ? 3 : 2));

	/* We've got a syllable <L,V,T?>; see if it can potentially be composed. */
	ಯದಿ (isCombiningL (l) && isCombiningV (v) && (t == 0 || isCombiningT (t)))
	{
	  /* Try to compose; if this succeeds, end is set to start+1. */
	  hb_codepoint_t s = SBase + (l - LBase) * NCount + (v - VBase) * TCount + tindex;
	  ಯದಿ (font->has_glyph (s))
	  {
	    (ಶೂನ್ಯ) buffer->replace_glyphs (t ? 3 : 2, 1, &s);
	    end = start + 1;
	    ಅನುವರ್ತ;
	  }
	}

	/* We didn't compose, either because it's an Old Hangul syllable without a
	 * precomposed character in Unicode, or because the font didn't support the
	 * necessary precomposed glyph.
	 * Set jamo features on the individual glyphs, and advance past them.
	 */
	buffer->cur().hangul_shaping_feature() = LJMO;
	(ಶೂನ್ಯ) buffer->next_glyph ();
	buffer->cur().hangul_shaping_feature() = VJMO;
	(ಶೂನ್ಯ) buffer->next_glyph ();
	ಯದಿ (t)
	{
	  buffer->cur().hangul_shaping_feature() = TJMO;
	  (ಶೂನ್ಯ) buffer->next_glyph ();
	  end = start + 3;
	}
	ಅನ್ಯಥಾ
	  end = start + 2;
	ಯದಿ (unlikely (!buffer->successful))
	  ಭಙ್ಗ;
	buffer->merge_out_grapheme_clusters (start, end);
	ಅನುವರ್ತ;
      }
    }

    ಅನ್ಯಥಾ ಯದಿ (isCombinedS (u))
    {
      /* Have <LV>, <LVT>, or <LV,T> */
      hb_codepoint_t s = u;
      ತಾರ್ಕಿಕ has_glyph = font->has_glyph (s);
      unsigned ಪೂರ್ಣಾಙ್ಕ lindex = (s - SBase) / NCount;
      unsigned ಪೂರ್ಣಾಙ್ಕ nindex = (s - SBase) % NCount;
      unsigned ಪೂರ್ಣಾಙ್ಕ vindex = nindex / TCount;
      unsigned ಪೂರ್ಣಾಙ್ಕ tindex = nindex % TCount;

      ಯದಿ (!tindex &&
	  buffer->idx + 1 < ಎಣಿಕೆ &&
	  isCombiningT (buffer->cur(+1).codepoint))
      {
	/* <LV,T>, try to combine. */
	unsigned ಪೂರ್ಣಾಙ್ಕ new_tindex = buffer->cur(+1).codepoint - TBase;
	hb_codepoint_t new_s = s + new_tindex;
	ಯದಿ (font->has_glyph (new_s))
	{
	  (ಶೂನ್ಯ) buffer->replace_glyphs (2, 1, &new_s);
	  end = start + 1;
	  ಅನುವರ್ತ;
	}
	ಅನ್ಯಥಾ
	  buffer->unsafe_to_break (buffer->idx, buffer->idx + 2); /* Mark unsafe between LV and T. */
      }

      /* Otherwise, decompose if font doesn't support <LV> or <LVT>,
       * or if having non-combining <LV,T>.  Note that we already handled
       * combining <LV,T> above. */
      ಯದಿ (!has_glyph ||
	  (!tindex &&
	   buffer->idx + 1 < ಎಣಿಕೆ &&
	   isT (buffer->cur(+1).codepoint)))
      {
	hb_codepoint_t decomposed[3] = {LBase + lindex,
					VBase + vindex,
					TBase + tindex};
	ಯದಿ (font->has_glyph (decomposed[0]) &&
	    font->has_glyph (decomposed[1]) &&
	    (!tindex || font->has_glyph (decomposed[2])))
	{
	  unsigned ಪೂರ್ಣಾಙ್ಕ s_len = tindex ? 3 : 2;
	  (ಶೂನ್ಯ) buffer->replace_glyphs (1, s_len, decomposed);

	  /* If we decomposed an LV because of a non-combining T following,
	   * we want to include this T in the syllable.
	   */
	  ಯದಿ (has_glyph && !tindex)
	  {
	    (ಶೂನ್ಯ) buffer->next_glyph ();
	    s_len++;
	  }
	  ಯದಿ (unlikely (!buffer->successful))
	    ಭಙ್ಗ;

	  /* We decomposed S: apply jamo features to the individual glyphs
	   * that are now in buffer->out_info.
	   */
	  hb_glyph_info_t *info = buffer->out_info;
	  end = start + s_len;

	  unsigned ಪೂರ್ಣಾಙ್ಕ i = start;
	  info[i++].hangul_shaping_feature() = LJMO;
	  info[i++].hangul_shaping_feature() = VJMO;
	  ಯದಿ (i < end)
	    info[i++].hangul_shaping_feature() = TJMO;

	  buffer->merge_out_grapheme_clusters (start, end);
	  ಅನುವರ್ತ;
	}
	ಅನ್ಯಥಾ ಯದಿ ((!tindex && buffer->idx + 1 < ಎಣಿಕೆ && isT (buffer->cur(+1).codepoint)))
	  buffer->unsafe_to_break (buffer->idx, buffer->idx + 2); /* Mark unsafe between LV and T. */
      }

      ಯದಿ (has_glyph)
      {
	/* We didn't decompose the S, so just advance past it and fall through. */
	end = start + 1;
      }
    }

    /* Didn't find a recognizable syllable, so we leave end <= start;
     * this will prevent tone-mark reordering happening.
     */
    (ಶೂನ್ಯ) buffer->next_glyph ();
  }
  buffer->ಸಮಕಾಲ ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
setup_masks_hangul (ಧ್ರುವ hb_ot_shape_plan_t *plan,
		    hb_buffer_t              *buffer,
		    hb_font_t                *font HB_UNUSED)
{
  ಧ್ರುವ hangul_shape_plan_t *hangul_plan = (ಧ್ರುವ hangul_shape_plan_t *) plan->data;

  ಯದಿ (likely (hangul_plan))
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++, info++)
    {
      info->mask |= hangul_plan->mask_array[info->hangul_shaping_feature()];

      /* Keep 'calt' away from jamo; see override_features_hangul(). */
      hb_codepoint_t u = info->codepoint;
      ಯದಿ (isL (u) || isV (u) || isT (u))
	info->mask &= ~hangul_plan->calt_mask;
    }
  }

  HB_BUFFER_DEALLOCATE_VAR (buffer, hangul_shaping_feature);
}


ಧ್ರುವ hb_ot_shaper_t _hb_ot_shaper_hangul =
{
  collect_features_hangul,
  override_features_hangul,
  data_create_hangul,
  data_destroy_hangul,
  preprocess_text_hangul,
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  nullptr, /* compose */
  setup_masks_hangul,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_NONE,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  ಸತ್ಯ, /* fallback_position */
};


#endif
