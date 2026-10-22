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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#समावेश "hb-ot-shape-normalize.hh"
#समावेश "hb-ot-shaper.hh"
#समावेश "hb-ot-shape.hh"


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


अटल अंतरभूत व्योम
set_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (व्योम) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_सूचकांक());
}

अटल अंतरभूत व्योम
output_char (hb_बफर_t *बफर, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  बफर->cur().normalizer_glyph_सूचकांक() = glyph;
  (व्योम) बफर->output_glyph (unichar);
  _hb_glyph_info_set_unicode_props (&बफर->prev(), बफर);
}

अटल अंतरभूत व्योम
अग्र_char (hb_बफर_t *बफर, hb_codepoint_t glyph)
{
  बफर->cur().normalizer_glyph_सूचकांक() = glyph;
  (व्योम) बफर->अग्र_glyph ();
}

अटल अंतरभूत व्योम
skip_char (hb_बफर_t *बफर)
{
  बफर->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
अटल अंतरभूत अचिन्हित पूर्णांक
decompose (स्थिर hb_ot_shape_normalize_context_t *c, bool shortest, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_बफर_t * स्थिर बफर = c->बफर;
  hb_font_t * स्थिर font = c->font;

  अगर (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    वापस 0;

  bool has_a = (bool) font->get_nominal_glyph (a, &a_glyph);
  अगर (shortest && has_a) {
    /* Output a and b */
    output_char (बफर, a, a_glyph);
    अगर (likely (b)) {
      output_char (बफर, b, b_glyph);
      वापस 2;
    }
    वापस 1;
  }

  अगर (अचिन्हित ret = decompose (c, shortest, a)) {
    अगर (b) {
      output_char (बफर, b, b_glyph);
      वापस ret + 1;
    }
    वापस ret;
  }

  अगर (has_a) {
    output_char (बफर, a, a_glyph);
    अगर (likely (b)) {
      output_char (बफर, b, b_glyph);
      वापस 2;
    }
    वापस 1;
  }

  वापस 0;
}

अटल अंतरभूत व्योम
decompose_current_character (स्थिर hb_ot_shape_normalize_context_t *c, bool shortest)
{
  hb_बफर_t * स्थिर बफर = c->बफर;
  hb_codepoint_t u = बफर->cur().codepoint;
  hb_codepoint_t glyph = 0;

  अगर (shortest && c->font->get_nominal_glyph (u, &glyph, बफर->not_found))
  {
    अग्र_char (बफर, glyph);
    वापस;
  }

  अगर (decompose (c, shortest, u))
  {
    skip_char (बफर);
    वापस;
  }

  अगर (!shortest && c->font->get_nominal_glyph (u, &glyph, बफर->not_found))
  {
    अग्र_char (बफर, glyph);
    वापस;
  }

  अगर (_hb_glyph_info_is_unicode_space (&बफर->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = बफर->unicode->space_fallback_type (u);
    अगर (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = बफर->invisible)))
    {
      _hb_glyph_info_set_unicode_space_fallback_type (&बफर->cur(), space_type);
      अग्र_char (बफर, space_glyph);
      बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
      वापस;
    }
  }

  अगर (u == 0x2011u)
  {
    /* U+2011 is the only sensible character that is a no-break version of another character
     * and not a space.  The space ones are handled already.  Handle this lone one. */
    hb_codepoint_t other_glyph;
    अगर (c->font->get_nominal_glyph (0x2010u, &other_glyph))
    {
      अग्र_char (बफर, other_glyph);
      वापस;
    }
  }

  अग्र_char (बफर, glyph); /* glyph is initialized in earlier branches. */
}

अटल अंतरभूत व्योम
handle_variation_selector_cluster (स्थिर hb_ot_shape_normalize_context_t *c,
				   अचिन्हित पूर्णांक end,
				   bool short_circuit HB_UNUSED)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_बफर_t * स्थिर बफर = c->बफर;
  hb_font_t * स्थिर font = c->font;
  क्रम (; बफर->idx < end - 1 && बफर->successful;) {
    अगर (unlikely (बफर->unicode->is_variation_selector (बफर->cur(+1).codepoint))) {
      अगर (font->get_variation_glyph (बफर->cur().codepoint, बफर->cur(+1).codepoint, &बफर->cur().normalizer_glyph_सूचकांक()))
      {
	hb_codepoint_t unicode = बफर->cur().codepoint;
	(व्योम) बफर->replace_glyphs (2, 1, &unicode);
      }
      अन्यथा
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	set_glyph (बफर->cur(), font);
	(व्योम) बफर->अग्र_glyph ();

        बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	_hb_glyph_info_set_variation_selector (&बफर->cur(), true);
	अगर (बफर->not_found_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&बफर->cur());

	set_glyph (बफर->cur(), font);
	(व्योम) बफर->अग्र_glyph ();
      }
      /* Skip any further variation selectors. */
      जबतक (बफर->idx < end &&
	     बफर->successful &&
	     unlikely (बफर->unicode->is_variation_selector (बफर->cur().codepoint)))
      {
	set_glyph (बफर->cur(), font);
	(व्योम) बफर->अग्र_glyph ();
      }
    }
    अन्यथा
    {
      set_glyph (बफर->cur(), font);
      (व्योम) बफर->अग्र_glyph ();
    }
  }
  अगर (likely (बफर->idx < end))
  {
    set_glyph (बफर->cur(), font);
    (व्योम) बफर->अग्र_glyph ();
  }
}

अटल अंतरभूत व्योम
decompose_multi_char_cluster (स्थिर hb_ot_shape_normalize_context_t *c, अचिन्हित पूर्णांक end, bool short_circuit)
{
  hb_बफर_t * स्थिर बफर = c->बफर;
  क्रम (अचिन्हित पूर्णांक i = बफर->idx; i < end && बफर->successful; i++)
    अगर (unlikely (बफर->unicode->is_variation_selector (बफर->info[i].codepoint))) {
      handle_variation_selector_cluster (c, end, short_circuit);
      वापस;
    }

  जबतक (बफर->idx < end && बफर->successful)
    decompose_current_character (c, short_circuit);
}


अटल पूर्णांक
compare_combining_class (स्थिर hb_glyph_info_t *pa, स्थिर hb_glyph_info_t *pb)
{
  अचिन्हित पूर्णांक a = _hb_glyph_info_get_modified_combining_class (pa);
  अचिन्हित पूर्णांक b = _hb_glyph_info_get_modified_combining_class (pb);

  वापस a < b ? -1 : a == b ? 0 : +1;
}


व्योम
_hb_ot_shape_normalize (स्थिर hb_ot_shape_plan_t *plan,
			hb_बफर_t *बफर,
			hb_font_t *font)
{
  अगर (unlikely (!बफर->len)) वापस;

  hb_बफर_assert_unicode_vars (बफर);

  hb_ot_shape_normalization_mode_t mode = plan->shaper->normalization_preference;
  अगर (mode == HB_OT_SHAPE_NORMALIZATION_MODE_AUTO)
  {
    अगर (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      mode = HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS;
    अन्यथा
      mode = HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS;
  }

  hb_ot_shape_normalize_context_t c = {
    plan,
    बफर,
    font,
    बफर->unicode,
    plan->shaper->decompose ? plan->shaper->decompose : hb_ot_shape_normalize_context_t::decompose_unicode,
    plan->shaper->compose   ? plan->shaper->compose   : hb_ot_shape_normalize_context_t::compose_unicode
  };
  c.override_decompose_and_compose (plan->shaper->decompose, plan->shaper->compose);

  bool always_short_circuit = mode == HB_OT_SHAPE_NORMALIZATION_MODE_NONE;
  bool might_short_circuit = always_short_circuit ||
			     (mode != HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED &&
			      mode != HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  अचिन्हित पूर्णांक गिनती;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  bool all_simple = true;
  अगर (बफर->message (font, "start decompose"))
  {
    बफर->clear_output ();
    गिनती = बफर->len;
    बफर->idx = 0;
    करो
    {
      अचिन्हित पूर्णांक end;
      क्रम (end = बफर->idx + 1; end < गिनती; end++)
	अगर (_hb_glyph_info_is_unicode_mark (&बफर->info[end]))
	  अवरोध;

      अगर (end < गिनती)
	end--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      अगर (might_short_circuit)
      {
	अचिन्हित पूर्णांक done = font->get_nominal_glyphs (end - बफर->idx,
						      &बफर->cur().codepoint,
						      माप (बफर->info[0]),
						      &बफर->cur().normalizer_glyph_सूचकांक(),
						      माप (बफर->info[0]));
	अगर (unlikely (!बफर->अग्र_glyphs (done))) अवरोध;
      }
      जबतक (बफर->idx < end && बफर->successful)
	decompose_current_character (&c, might_short_circuit);

      अगर (बफर->idx == गिनती || !बफर->successful)
	अवरोध;

      all_simple = false;

      /* Find all the marks now. */
      क्रम (end = बफर->idx + 1; end < गिनती; end++)
	अगर (!_hb_glyph_info_is_unicode_mark(&बफर->info[end]))
	  अवरोध;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, end, always_short_circuit);
    }
    जबतक (बफर->idx < गिनती && बफर->successful);
    बफर->sync ();
    (व्योम) बफर->message (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  अगर (!all_simple && बफर->message(font, "start reorder"))
  {
    गिनती = बफर->len;
    hb_glyph_info_t *info = बफर->info;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	जारी;

      अचिन्हित पूर्णांक end;
      क्रम (end = i + 1; end < गिनती; end++)
	अगर (_hb_glyph_info_get_modified_combining_class (&info[end]) == 0)
	  अवरोध;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      अगर (end - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = end;
	जारी;
      }

      बफर->sort (i, end, compare_combining_class);

      अगर (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, बफर, i, end);

      i = end;
    }
    (व्योम) बफर->message(font, "end reorder");
  }
  अगर (बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    अचिन्हित गिनती = बफर->len;
    hb_glyph_info_t *info = बफर->info;
    क्रम (अचिन्हित पूर्णांक i = 1; i + 1 < गिनती; i++)
      अगर (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  अगर (!all_simple &&
      बफर->successful &&
      (mode == HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS ||
       mode == HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      बफर->message (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    बफर->clear_output ();
    गिनती = बफर->len;
    अचिन्हित पूर्णांक starter = 0;
    (व्योम) बफर->अग्र_glyph ();
    जबतक (बफर->idx < गिनती /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      अगर (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&बफर->cur()))
      {
	अगर (/* If there's anything between the starter and this char, they should have CCC
	     * smaller than this character's. */
	    (starter == बफर->out_len - 1 ||
	     info_cc (बफर->prev()) < info_cc (बफर->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       बफर->out_info[starter].codepoint,
		       बफर->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  अगर (unlikely (!बफर->अग्र_glyph ())) अवरोध; /* Copy to out-buffer. */
	  बफर->merge_out_clusters (starter, बफर->out_len);
	  बफर->out_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  बफर->out_info[starter].codepoint = composed;
	  बफर->out_info[starter].normalizer_glyph_सूचकांक() = glyph;
	  _hb_glyph_info_set_unicode_props (&बफर->out_info[starter], बफर);

	  जारी;
	}
      }

      /* Blocked, or doesn't compose. */
      अगर (unlikely (!बफर->अग्र_glyph ())) अवरोध;

      अगर (info_cc (बफर->prev()) == 0)
	starter = बफर->out_len - 1;
    }
    बफर->sync ();
    (व्योम) बफर->message (font, "end compose");
  }
}


#पूर्ण_अगर
