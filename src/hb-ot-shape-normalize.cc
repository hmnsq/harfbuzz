/*
 * Copyright © 2011,2012  Google, Inc.
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

#شامل "hb-ot-shape-normalize.hh"
#شامل "hb-ot-shaper.hh"
#شامل "hb-ot-shape.hh"


/*
 * HIGHLEVEL DESIGN:
 *
 * This file exports one main function: _hb_ot_shape_normalize().
 *
 * This function closely reflects the Unicode Normalization Algorithm,
 * yet it's different.
 *
 * Each shaper specifies whether it prefers decomposed (NFD) or composed (NFC).
 * The logic however tries to use whatever the font can support.
 *
 * In general what happens is that: each grapheme is decomposed in a chain
 * of 1:2 decompositions, marks reordered, and then recomposed if desired,
 * so far it's like Unicode Normalization.  However, the decomposition and
 * recomposition only happens if the font supports the resulting characters.
 *
 * The goals are:
 *
 *   - Try to render all canonically equivalent strings similarly.  To really
 *     achieve this we have to always do the full decomposition and then
 *     selectively recompose from there.  It's kinda too expensive though, so
 *     we skip some cases.  For example, if composed is desired, we simply
 *     don't touch 1-character clusters that are supported by the font, even
 *     though their NFC may be different.
 *
 *   - When a font has a precomposed character for a sequence but the 'ccmp'
 *     feature in the font is not adequate, use the precomposed character
 *     which typically has better mark positioning.
 *
 *   - When a font does not support a combining mark, but supports it precomposed
 *     with previous base, use that.  This needs the itemizer to have this
 *     knowledge too.  We need to provide assistance to the itemizer.
 *
 *   - When a font does not support a character but supports its canonical
 *     decomposition, well, use the decomposition.
 *
 *   - The shapers can customize the compose and decompose functions to
 *     offload some of their requirements to the normalizer.  For example, the
 *     Indic shaper may want to disallow recomposing of two matras.
 */


ساکن اندرونی خالی
سیٹ_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (خالی) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_اشاریہ());
}

ساکن اندرونی خالی
output_char (hb_بفر_t *بفر, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  بفر->cur().normalizer_glyph_اشاریہ() = glyph;
  (خالی) بفر->output_glyph (unichar);
  hb_glyph_info_سیٹ_unicode_props (&بفر->prev(), بفر);
}

ساکن اندرونی خالی
اگلا_char (hb_بفر_t *بفر, hb_codepoint_t glyph)
{
  بفر->cur().normalizer_glyph_اشاریہ() = glyph;
  (خالی) بفر->اگلا_glyph ();
}

ساکن اندرونی خالی
skip_char (hb_بفر_t *بفر)
{
  بفر->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
ساکن اندرونی غیرعلامتی صحیح
decompose (ثابت hb_ot_shape_normalize_context_t *c, منطقی مختصر_ترین, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_font_t * ثابت font = c->font;

  اگر (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    واپس 0;

  منطقی has_a = (منطقی) font->get_nominal_glyph (a, &a_glyph);
  اگر (مختصر_ترین && has_a) {
    /* Output a and b */
    output_char (بفر, a, a_glyph);
    اگر (likely (b)) {
      output_char (بفر, b, b_glyph);
      واپس 2;
    }
    واپس 1;
  }

  اگر (غیرعلامتی ret = decompose (c, مختصر_ترین, a)) {
    اگر (b) {
      output_char (بفر, b, b_glyph);
      واپس ret + 1;
    }
    واپس ret;
  }

  اگر (has_a) {
    output_char (بفر, a, a_glyph);
    اگر (likely (b)) {
      output_char (بفر, b, b_glyph);
      واپس 2;
    }
    واپس 1;
  }

  واپس 0;
}

ساکن اندرونی خالی
decompose_current_character (ثابت hb_ot_shape_normalize_context_t *c, منطقی مختصر_ترین)
{
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_codepoint_t u = بفر->cur().codepoint;
  hb_codepoint_t glyph = 0;

  اگر (مختصر_ترین && c->font->get_nominal_glyph (u, &glyph, بفر->not_ملا))
  {
    اگلا_char (بفر, glyph);
    واپس;
  }

  اگر (decompose (c, مختصر_ترین, u))
  {
    skip_char (بفر);
    واپس;
  }

  اگر (!مختصر_ترین && c->font->get_nominal_glyph (u, &glyph, بفر->not_ملا))
  {
    اگلا_char (بفر, glyph);
    واپس;
  }

  اگر (_hb_glyph_info_is_unicode_space (&بفر->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = بفر->unicode->space_fallback_type (u);
    اگر (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = بفر->invisible)))
    {
      hb_glyph_info_سیٹ_unicode_space_fallback_type (&بفر->cur(), space_type);
      اگلا_char (بفر, space_glyph);
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
      واپس;
    }
  }

  اگر (u == 0x2011u)
  {
    /* U+2011 is the only sensible character that is a no-break version of another character
     * and not a space.  The space ones are handled already.  Handle this lone one. */
    hb_codepoint_t other_glyph;
    اگر (c->font->get_nominal_glyph (0x2010u, &other_glyph))
    {
      اگلا_char (بفر, other_glyph);
      واپس;
    }
  }

  اگلا_char (بفر, glyph); /* glyph is initialized in earlier branches. */
}

ساکن اندرونی خالی
handle_variation_selector_cluster (ثابت hb_ot_shape_normalize_context_t *c,
				   غیرعلامتی صحیح انتہا,
				   منطقی short_circuit HB_غیر_مستعمل)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_font_t * ثابت font = c->font;
  برائے (; بفر->idx < انتہا - 1 && بفر->successful;) {
    اگر (unlikely (بفر->unicode->is_variation_selector (بفر->cur(+1).codepoint))) {
      اگر (font->get_variation_glyph (بفر->cur().codepoint, بفر->cur(+1).codepoint, &بفر->cur().normalizer_glyph_اشاریہ()))
      {
	hb_codepoint_t unicode = بفر->cur().codepoint;
	(خالی) بفر->replace_glyphs (2, 1, &unicode);
      }
      ورنہ
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	سیٹ_glyph (بفر->cur(), font);
	(خالی) بفر->اگلا_glyph ();

        بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	hb_glyph_info_سیٹ_variation_selector (&بفر->cur(), سچ);
	اگر (بفر->not_ملا_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&بفر->cur());

	سیٹ_glyph (بفر->cur(), font);
	(خالی) بفر->اگلا_glyph ();
      }
      /* Skip any further variation selectors. */
      جب (بفر->idx < انتہا &&
	     بفر->successful &&
	     unlikely (بفر->unicode->is_variation_selector (بفر->cur().codepoint)))
      {
	سیٹ_glyph (بفر->cur(), font);
	(خالی) بفر->اگلا_glyph ();
      }
    }
    ورنہ
    {
      سیٹ_glyph (بفر->cur(), font);
      (خالی) بفر->اگلا_glyph ();
    }
  }
  اگر (likely (بفر->idx < انتہا))
  {
    سیٹ_glyph (بفر->cur(), font);
    (خالی) بفر->اگلا_glyph ();
  }
}

ساکن اندرونی خالی
decompose_multi_char_cluster (ثابت hb_ot_shape_normalize_context_t *c, غیرعلامتی صحیح انتہا, منطقی short_circuit)
{
  hb_بفر_t * ثابت بفر = c->بفر;
  برائے (غیرعلامتی صحیح i = بفر->idx; i < انتہا && بفر->successful; i++)
    اگر (unlikely (بفر->unicode->is_variation_selector (بفر->info[i].codepoint))) {
      handle_variation_selector_cluster (c, انتہا, short_circuit);
      واپس;
    }

  جب (بفر->idx < انتہا && بفر->successful)
    decompose_current_character (c, short_circuit);
}


ساکن صحیح
compare_combining_class (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  غیرعلامتی صحیح a = _hb_glyph_info_get_modified_combining_class (pa);
  غیرعلامتی صحیح b = _hb_glyph_info_get_modified_combining_class (pb);

  واپس a < b ? -1 : a == b ? 0 : +1;
}


خالی
_hb_ot_shape_normalize (ثابت hb_ot_shape_plan_t *plan,
			hb_بفر_t *بفر,
			hb_font_t *font)
{
  اگر (unlikely (!بفر->len)) واپس;

  hb_بفر_assert_unicode_vars (بفر);

  hb_ot_shape_normalization_منوال_t منوال = plan->shaper->normalization_preference;
  اگر (منوال == HB_OT_SHAPE_NORMALIZATION_منوال_AUTO)
  {
    اگر (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      منوال = HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS;
    ورنہ
      منوال = HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS;
  }

  hb_ot_shape_normalize_context_t c = {
    plan,
    بفر,
    font,
    بفر->unicode,
    plan->shaper->decompose ? plan->shaper->decompose : hb_ot_shape_normalize_context_t::decompose_unicode,
    plan->shaper->compose   ? plan->shaper->compose   : hb_ot_shape_normalize_context_t::compose_unicode
  };
  c.override_decompose_and_compose (plan->shaper->decompose, plan->shaper->compose);

  منطقی always_short_circuit = منوال == HB_OT_SHAPE_NORMALIZATION_منوال_NONE;
  منطقی might_short_circuit = always_short_circuit ||
			     (منوال != HB_OT_SHAPE_NORMALIZATION_منوال_DECOMPOSED &&
			      منوال != HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  غیرعلامتی صحیح گنتی;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  منطقی all_simple = سچ;
  اگر (بفر->پیغام (font, "start decompose"))
  {
    بفر->clear_output ();
    گنتی = بفر->len;
    بفر->idx = 0;
    کرو
    {
      غیرعلامتی صحیح انتہا;
      برائے (انتہا = بفر->idx + 1; انتہا < گنتی; انتہا++)
	اگر (_hb_glyph_info_is_unicode_mark (&بفر->info[انتہا]))
	  توڑو;

      اگر (انتہا < گنتی)
	انتہا--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      اگر (might_short_circuit)
      {
	غیرعلامتی صحیح مکمل = font->get_nominal_glyphs (انتہا - بفر->idx,
						      &بفر->cur().codepoint,
						      حجم (بفر->info[0]),
						      &بفر->cur().normalizer_glyph_اشاریہ(),
						      حجم (بفر->info[0]));
	اگر (unlikely (!بفر->اگلا_glyphs (مکمل))) توڑو;
      }
      جب (بفر->idx < انتہا && بفر->successful)
	decompose_current_character (&c, might_short_circuit);

      اگر (بفر->idx == گنتی || !بفر->successful)
	توڑو;

      all_simple = جھوٹ;

      /* Find all the marks now. */
      برائے (انتہا = بفر->idx + 1; انتہا < گنتی; انتہا++)
	اگر (!_hb_glyph_info_is_unicode_mark(&بفر->info[انتہا]))
	  توڑو;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, انتہا, always_short_circuit);
    }
    جب (بفر->idx < گنتی && بفر->successful);
    بفر->ہمآہنگ ();
    (خالی) بفر->پیغام (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  اگر (!all_simple && بفر->پیغام(font, "start reorder"))
  {
    گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	جاری;

      غیرعلامتی صحیح انتہا;
      برائے (انتہا = i + 1; انتہا < گنتی; انتہا++)
	اگر (_hb_glyph_info_get_modified_combining_class (&info[انتہا]) == 0)
	  توڑو;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      اگر (انتہا - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = انتہا;
	جاری;
      }

      بفر->sort (i, انتہا, compare_combining_class);

      اگر (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, بفر, i, انتہا);

      i = انتہا;
    }
    (خالی) بفر->پیغام(font, "end reorder");
  }
  اگر (بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    غیرعلامتی گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    برائے (غیرعلامتی صحیح i = 1; i + 1 < گنتی; i++)
      اگر (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  اگر (!all_simple &&
      بفر->successful &&
      (منوال == HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS ||
       منوال == HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      بفر->پیغام (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    بفر->clear_output ();
    گنتی = بفر->len;
    غیرعلامتی صحیح starter = 0;
    (خالی) بفر->اگلا_glyph ();
    جب (بفر->idx < گنتی /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      اگر (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&بفر->cur()))
      {
	اگر (/* If there's anything between the starter and this char, they should have CCC
	     * smaller than this character's. */
	    (starter == بفر->برآمد_len - 1 ||
	     info_cc (بفر->prev()) < info_cc (بفر->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       بفر->برآمد_info[starter].codepoint,
		       بفر->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  اگر (unlikely (!بفر->اگلا_glyph ())) توڑو; /* Copy to out-buffer. */
	  بفر->ملاؤ_برآمد_clusters (starter, بفر->برآمد_len);
	  بفر->برآمد_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  بفر->برآمد_info[starter].codepoint = composed;
	  بفر->برآمد_info[starter].normalizer_glyph_اشاریہ() = glyph;
	  hb_glyph_info_سیٹ_unicode_props (&بفر->برآمد_info[starter], بفر);

	  جاری;
	}
      }

      /* Blocked, or doesn't compose. */
      اگر (unlikely (!بفر->اگلا_glyph ())) توڑو;

      اگر (info_cc (بفر->prev()) == 0)
	starter = بفر->برآمد_len - 1;
    }
    بفر->ہمآہنگ ();
    (خالی) بفر->پیغام (font, "end compose");
  }
}


#اگر_ختم
