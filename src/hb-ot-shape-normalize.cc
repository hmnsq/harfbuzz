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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shape-normalize.hh"
#అన్తర్భావ "hb-ot-shaper.hh"
#అన్తర్భావ "hb-ot-shape.hh"


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


స్థిర అంతఃవరుస శూన్య
సమితి_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (శూన్య) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_సూచీ());
}

స్థిర అంతఃవరుస శూన్య
output_char (hb_బఫరు_t *బఫరు, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  బఫరు->cur().normalizer_glyph_సూచీ() = glyph;
  (శూన్య) బఫరు->output_glyph (unichar);
  hb_glyph_info_సమితి_unicode_props (&బఫరు->prev(), బఫరు);
}

స్థిర అంతఃవరుస శూన్య
తదుపరి_char (hb_బఫరు_t *బఫరు, hb_codepoint_t glyph)
{
  బఫరు->cur().normalizer_glyph_సూచీ() = glyph;
  (శూన్య) బఫరు->తదుపరి_glyph ();
}

స్థిర అంతఃవరుస శూన్య
skip_char (hb_బఫరు_t *బఫరు)
{
  బఫరు->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
decompose (స్థిరరాశి hb_ot_shape_normalize_context_t *c, తార్కిక అతితక్కువ, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_బఫరు_t * స్థిరరాశి బఫరు = c->బఫరు;
  hb_font_t * స్థిరరాశి font = c->font;

  ఒకవేళ (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    తిరిగి 0;

  తార్కిక has_a = (తార్కిక) font->get_nominal_glyph (a, &a_glyph);
  ఒకవేళ (అతితక్కువ && has_a) {
    /* Output a and b */
    output_char (బఫరు, a, a_glyph);
    ఒకవేళ (likely (b)) {
      output_char (బఫరు, b, b_glyph);
      తిరిగి 2;
    }
    తిరిగి 1;
  }

  ఒకవేళ (గుర్తులేని ret = decompose (c, అతితక్కువ, a)) {
    ఒకవేళ (b) {
      output_char (బఫరు, b, b_glyph);
      తిరిగి ret + 1;
    }
    తిరిగి ret;
  }

  ఒకవేళ (has_a) {
    output_char (బఫరు, a, a_glyph);
    ఒకవేళ (likely (b)) {
      output_char (బఫరు, b, b_glyph);
      తిరిగి 2;
    }
    తిరిగి 1;
  }

  తిరిగి 0;
}

స్థిర అంతఃవరుస శూన్య
decompose_current_character (స్థిరరాశి hb_ot_shape_normalize_context_t *c, తార్కిక అతితక్కువ)
{
  hb_బఫరు_t * స్థిరరాశి బఫరు = c->బఫరు;
  hb_codepoint_t u = బఫరు->cur().codepoint;
  hb_codepoint_t glyph = 0;

  ఒకవేళ (అతితక్కువ && c->font->get_nominal_glyph (u, &glyph, బఫరు->not_దొరికింది))
  {
    తదుపరి_char (బఫరు, glyph);
    తిరిగి;
  }

  ఒకవేళ (decompose (c, అతితక్కువ, u))
  {
    skip_char (బఫరు);
    తిరిగి;
  }

  ఒకవేళ (!అతితక్కువ && c->font->get_nominal_glyph (u, &glyph, బఫరు->not_దొరికింది))
  {
    తదుపరి_char (బఫరు, glyph);
    తిరిగి;
  }

  ఒకవేళ (_hb_glyph_info_is_unicode_space (&బఫరు->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = బఫరు->unicode->space_fallback_type (u);
    ఒకవేళ (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = బఫరు->invisible)))
    {
      hb_glyph_info_సమితి_unicode_space_fallback_type (&బఫరు->cur(), space_type);
      తదుపరి_char (బఫరు, space_glyph);
      బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
      తిరిగి;
    }
  }

  ఒకవేళ (u == 0x2011u)
  {
    /* U+2011 is the only sensible character that is a no-break version of another character
     * and not a space.  The space ones are handled already.  Handle this lone one. */
    hb_codepoint_t other_glyph;
    ఒకవేళ (c->font->get_nominal_glyph (0x2010u, &other_glyph))
    {
      తదుపరి_char (బఫరు, other_glyph);
      తిరిగి;
    }
  }

  తదుపరి_char (బఫరు, glyph); /* glyph is initialized in earlier branches. */
}

స్థిర అంతఃవరుస శూన్య
handle_variation_selector_cluster (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
				   గుర్తులేని పూర్ణాంకం చివర,
				   తార్కిక short_circuit HB_వాడని)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_బఫరు_t * స్థిరరాశి బఫరు = c->బఫరు;
  hb_font_t * స్థిరరాశి font = c->font;
  కొరకు (; బఫరు->idx < చివర - 1 && బఫరు->successful;) {
    ఒకవేళ (unlikely (బఫరు->unicode->is_variation_selector (బఫరు->cur(+1).codepoint))) {
      ఒకవేళ (font->get_variation_glyph (బఫరు->cur().codepoint, బఫరు->cur(+1).codepoint, &బఫరు->cur().normalizer_glyph_సూచీ()))
      {
	hb_codepoint_t unicode = బఫరు->cur().codepoint;
	(శూన్య) బఫరు->replace_glyphs (2, 1, &unicode);
      }
      లేకపోతే
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	సమితి_glyph (బఫరు->cur(), font);
	(శూన్య) బఫరు->తదుపరి_glyph ();

        బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	hb_glyph_info_సమితి_variation_selector (&బఫరు->cur(), నిజం);
	ఒకవేళ (బఫరు->not_దొరికింది_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&బఫరు->cur());

	సమితి_glyph (బఫరు->cur(), font);
	(శూన్య) బఫరు->తదుపరి_glyph ();
      }
      /* Skip any further variation selectors. */
      అయితే (బఫరు->idx < చివర &&
	     బఫరు->successful &&
	     unlikely (బఫరు->unicode->is_variation_selector (బఫరు->cur().codepoint)))
      {
	సమితి_glyph (బఫరు->cur(), font);
	(శూన్య) బఫరు->తదుపరి_glyph ();
      }
    }
    లేకపోతే
    {
      సమితి_glyph (బఫరు->cur(), font);
      (శూన్య) బఫరు->తదుపరి_glyph ();
    }
  }
  ఒకవేళ (likely (బఫరు->idx < చివర))
  {
    సమితి_glyph (బఫరు->cur(), font);
    (శూన్య) బఫరు->తదుపరి_glyph ();
  }
}

స్థిర అంతఃవరుస శూన్య
decompose_multi_char_cluster (స్థిరరాశి hb_ot_shape_normalize_context_t *c, గుర్తులేని పూర్ణాంకం చివర, తార్కిక short_circuit)
{
  hb_బఫరు_t * స్థిరరాశి బఫరు = c->బఫరు;
  కొరకు (గుర్తులేని పూర్ణాంకం i = బఫరు->idx; i < చివర && బఫరు->successful; i++)
    ఒకవేళ (unlikely (బఫరు->unicode->is_variation_selector (బఫరు->info[i].codepoint))) {
      handle_variation_selector_cluster (c, చివర, short_circuit);
      తిరిగి;
    }

  అయితే (బఫరు->idx < చివర && బఫరు->successful)
    decompose_current_character (c, short_circuit);
}


స్థిర పూర్ణాంకం
compare_combining_class (స్థిరరాశి hb_glyph_info_t *pa, స్థిరరాశి hb_glyph_info_t *pb)
{
  గుర్తులేని పూర్ణాంకం a = _hb_glyph_info_get_modified_combining_class (pa);
  గుర్తులేని పూర్ణాంకం b = _hb_glyph_info_get_modified_combining_class (pb);

  తిరిగి a < b ? -1 : a == b ? 0 : +1;
}


శూన్య
_hb_ot_shape_normalize (స్థిరరాశి hb_ot_shape_plan_t *plan,
			hb_బఫరు_t *బఫరు,
			hb_font_t *font)
{
  ఒకవేళ (unlikely (!బఫరు->len)) తిరిగి;

  hb_బఫరు_assert_unicode_vars (బఫరు);

  hb_ot_shape_normalization_బహుళకం_t బహుళకం = plan->shaper->normalization_preference;
  ఒకవేళ (బహుళకం == HB_OT_SHAPE_NORMALIZATION_బహుళకం_AUTO)
  {
    ఒకవేళ (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      బహుళకం = HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS;
    లేకపోతే
      బహుళకం = HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS;
  }

  hb_ot_shape_normalize_context_t c = {
    plan,
    బఫరు,
    font,
    బఫరు->unicode,
    plan->shaper->decompose ? plan->shaper->decompose : hb_ot_shape_normalize_context_t::decompose_unicode,
    plan->shaper->compose   ? plan->shaper->compose   : hb_ot_shape_normalize_context_t::compose_unicode
  };
  c.override_decompose_and_compose (plan->shaper->decompose, plan->shaper->compose);

  తార్కిక always_short_circuit = బహుళకం == HB_OT_SHAPE_NORMALIZATION_బహుళకం_NONE;
  తార్కిక might_short_circuit = always_short_circuit ||
			     (బహుళకం != HB_OT_SHAPE_NORMALIZATION_బహుళకం_DECOMPOSED &&
			      బహుళకం != HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  గుర్తులేని పూర్ణాంకం లెక్క;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  తార్కిక all_simple = నిజం;
  ఒకవేళ (బఫరు->సందేశం (font, "start decompose"))
  {
    బఫరు->clear_output ();
    లెక్క = బఫరు->len;
    బఫరు->idx = 0;
    చేయి
    {
      గుర్తులేని పూర్ణాంకం చివర;
      కొరకు (చివర = బఫరు->idx + 1; చివర < లెక్క; చివర++)
	ఒకవేళ (_hb_glyph_info_is_unicode_mark (&బఫరు->info[చివర]))
	  ఆపు;

      ఒకవేళ (చివర < లెక్క)
	చివర--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      ఒకవేళ (might_short_circuit)
      {
	గుర్తులేని పూర్ణాంకం పూర్తి = font->get_nominal_glyphs (చివర - బఫరు->idx,
						      &బఫరు->cur().codepoint,
						      పరిమాణం (బఫరు->info[0]),
						      &బఫరు->cur().normalizer_glyph_సూచీ(),
						      పరిమాణం (బఫరు->info[0]));
	ఒకవేళ (unlikely (!బఫరు->తదుపరి_glyphs (పూర్తి))) ఆపు;
      }
      అయితే (బఫరు->idx < చివర && బఫరు->successful)
	decompose_current_character (&c, might_short_circuit);

      ఒకవేళ (బఫరు->idx == లెక్క || !బఫరు->successful)
	ఆపు;

      all_simple = అబద్ధం;

      /* Find all the marks now. */
      కొరకు (చివర = బఫరు->idx + 1; చివర < లెక్క; చివర++)
	ఒకవేళ (!_hb_glyph_info_is_unicode_mark(&బఫరు->info[చివర]))
	  ఆపు;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, చివర, always_short_circuit);
    }
    అయితే (బఫరు->idx < లెక్క && బఫరు->successful);
    బఫరు->సమకాలీకరణ ();
    (శూన్య) బఫరు->సందేశం (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  ఒకవేళ (!all_simple && బఫరు->సందేశం(font, "start reorder"))
  {
    లెక్క = బఫరు->len;
    hb_glyph_info_t *info = బఫరు->info;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	కొనసాగు;

      గుర్తులేని పూర్ణాంకం చివర;
      కొరకు (చివర = i + 1; చివర < లెక్క; చివర++)
	ఒకవేళ (_hb_glyph_info_get_modified_combining_class (&info[చివర]) == 0)
	  ఆపు;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      ఒకవేళ (చివర - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = చివర;
	కొనసాగు;
      }

      బఫరు->sort (i, చివర, compare_combining_class);

      ఒకవేళ (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, బఫరు, i, చివర);

      i = చివర;
    }
    (శూన్య) బఫరు->సందేశం(font, "end reorder");
  }
  ఒకవేళ (బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    గుర్తులేని లెక్క = బఫరు->len;
    hb_glyph_info_t *info = బఫరు->info;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i + 1 < లెక్క; i++)
      ఒకవేళ (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  ఒకవేళ (!all_simple &&
      బఫరు->successful &&
      (బహుళకం == HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS ||
       బహుళకం == HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      బఫరు->సందేశం (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    బఫరు->clear_output ();
    లెక్క = బఫరు->len;
    గుర్తులేని పూర్ణాంకం starter = 0;
    (శూన్య) బఫరు->తదుపరి_glyph ();
    అయితే (బఫరు->idx < లెక్క /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      ఒకవేళ (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&బఫరు->cur()))
      {
	ఒకవేళ (/* If there's anything between the starter and this char, they should have CCC
	     * smaller than this character's. */
	    (starter == బఫరు->బహిర్గతం_len - 1 ||
	     info_cc (బఫరు->prev()) < info_cc (బఫరు->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       బఫరు->బహిర్గతం_info[starter].codepoint,
		       బఫరు->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  ఒకవేళ (unlikely (!బఫరు->తదుపరి_glyph ())) ఆపు; /* Copy to out-buffer. */
	  బఫరు->విలీనం_బహిర్గతం_clusters (starter, బఫరు->బహిర్గతం_len);
	  బఫరు->బహిర్గతం_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  బఫరు->బహిర్గతం_info[starter].codepoint = composed;
	  బఫరు->బహిర్గతం_info[starter].normalizer_glyph_సూచీ() = glyph;
	  hb_glyph_info_సమితి_unicode_props (&బఫరు->బహిర్గతం_info[starter], బఫరు);

	  కొనసాగు;
	}
      }

      /* Blocked, or doesn't compose. */
      ఒకవేళ (unlikely (!బఫరు->తదుపరి_glyph ())) ఆపు;

      ఒకవేళ (info_cc (బఫరు->prev()) == 0)
	starter = బఫరు->బహిర్గతం_len - 1;
    }
    బఫరు->సమకాలీకరణ ();
    (శూన్య) బఫరు->సందేశం (font, "end compose");
  }
}


#అయితే_ముగింపు
