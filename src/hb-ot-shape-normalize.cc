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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ଅନ୍ତର୍ଭାଵ "hb-ot-shape-normalize.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shape.hh"


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


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
set_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (ଶୂନ୍ୟ) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_index());
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
output_char (hb_buffer_t *buffer, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  buffer->cur().normalizer_glyph_index() = glyph;
  (ଶୂନ୍ୟ) buffer->output_glyph (unichar);
  _hb_glyph_info_set_unicode_props (&buffer->prev(), buffer);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
next_char (hb_buffer_t *buffer, hb_codepoint_t glyph)
{
  buffer->cur().normalizer_glyph_index() = glyph;
  (ଶୂନ୍ୟ) buffer->next_glyph ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
skip_char (hb_buffer_t *buffer)
{
  buffer->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
decompose (ଅଚଳ hb_ot_shape_normalize_context_t *c, ତାର୍କିକ shortest, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_buffer_t * ଅଚଳ buffer = c->buffer;
  hb_font_t * ଅଚଳ font = c->font;

  ଯଦି (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    ଫେରନ୍ତୁ 0;

  ତାର୍କିକ has_a = (ତାର୍କିକ) font->get_nominal_glyph (a, &a_glyph);
  ଯଦି (shortest && has_a) {
    /* Output a and b */
    output_char (buffer, a, a_glyph);
    ଯଦି (likely (b)) {
      output_char (buffer, b, b_glyph);
      ଫେରନ୍ତୁ 2;
    }
    ଫେରନ୍ତୁ 1;
  }

  ଯଦି (unsigned ret = decompose (c, shortest, a)) {
    ଯଦି (b) {
      output_char (buffer, b, b_glyph);
      ଫେରନ୍ତୁ ret + 1;
    }
    ଫେରନ୍ତୁ ret;
  }

  ଯଦି (has_a) {
    output_char (buffer, a, a_glyph);
    ଯଦି (likely (b)) {
      output_char (buffer, b, b_glyph);
      ଫେରନ୍ତୁ 2;
    }
    ଫେରନ୍ତୁ 1;
  }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
decompose_current_character (ଅଚଳ hb_ot_shape_normalize_context_t *c, ତାର୍କିକ shortest)
{
  hb_buffer_t * ଅଚଳ buffer = c->buffer;
  hb_codepoint_t u = buffer->cur().codepoint;
  hb_codepoint_t glyph = 0;

  ଯଦି (shortest && c->font->get_nominal_glyph (u, &glyph, buffer->not_found))
  {
    next_char (buffer, glyph);
    ଫେରନ୍ତୁ;
  }

  ଯଦି (decompose (c, shortest, u))
  {
    skip_char (buffer);
    ଫେରନ୍ତୁ;
  }

  ଯଦି (!shortest && c->font->get_nominal_glyph (u, &glyph, buffer->not_found))
  {
    next_char (buffer, glyph);
    ଫେରନ୍ତୁ;
  }

  ଯଦି (_hb_glyph_info_is_unicode_space (&buffer->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = buffer->unicode->space_fallback_type (u);
    ଯଦି (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = buffer->invisible)))
    {
      _hb_glyph_info_set_unicode_space_fallback_type (&buffer->cur(), space_type);
      next_char (buffer, space_glyph);
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
      ଫେରନ୍ତୁ;
    }
  }

  ଯଦି (u == 0x2011u)
  {
    /* U+2011 is the only sensible character that is a no-break version of another character
     * and not a space.  The space ones are handled already.  Handle this lone one. */
    hb_codepoint_t other_glyph;
    ଯଦି (c->font->get_nominal_glyph (0x2010u, &other_glyph))
    {
      next_char (buffer, other_glyph);
      ଫେରନ୍ତୁ;
    }
  }

  next_char (buffer, glyph); /* glyph is initialized in earlier branches. */
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
handle_variation_selector_cluster (ଅଚଳ hb_ot_shape_normalize_context_t *c,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end,
				   ତାର୍କିକ short_circuit HB_UNUSED)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_buffer_t * ଅଚଳ buffer = c->buffer;
  hb_font_t * ଅଚଳ font = c->font;
  ପାଇଁ (; buffer->idx < end - 1 && buffer->successful;) {
    ଯଦି (unlikely (buffer->unicode->is_variation_selector (buffer->cur(+1).codepoint))) {
      ଯଦି (font->get_variation_glyph (buffer->cur().codepoint, buffer->cur(+1).codepoint, &buffer->cur().normalizer_glyph_index()))
      {
	hb_codepoint_t unicode = buffer->cur().codepoint;
	(ଶୂନ୍ୟ) buffer->replace_glyphs (2, 1, &unicode);
      }
      ଅନ୍ୟଥା
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	set_glyph (buffer->cur(), font);
	(ଶୂନ୍ୟ) buffer->next_glyph ();

        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	_hb_glyph_info_set_variation_selector (&buffer->cur(), ସତ୍ୟ);
	ଯଦି (buffer->not_found_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&buffer->cur());

	set_glyph (buffer->cur(), font);
	(ଶୂନ୍ୟ) buffer->next_glyph ();
      }
      /* Skip any further variation selectors. */
      ଯେତେବେଳେ (buffer->idx < end &&
	     buffer->successful &&
	     unlikely (buffer->unicode->is_variation_selector (buffer->cur().codepoint)))
      {
	set_glyph (buffer->cur(), font);
	(ଶୂନ୍ୟ) buffer->next_glyph ();
      }
    }
    ଅନ୍ୟଥା
    {
      set_glyph (buffer->cur(), font);
      (ଶୂନ୍ୟ) buffer->next_glyph ();
    }
  }
  ଯଦି (likely (buffer->idx < end))
  {
    set_glyph (buffer->cur(), font);
    (ଶୂନ୍ୟ) buffer->next_glyph ();
  }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
decompose_multi_char_cluster (ଅଚଳ hb_ot_shape_normalize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end, ତାର୍କିକ short_circuit)
{
  hb_buffer_t * ଅଚଳ buffer = c->buffer;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = buffer->idx; i < end && buffer->successful; i++)
    ଯଦି (unlikely (buffer->unicode->is_variation_selector (buffer->info[i].codepoint))) {
      handle_variation_selector_cluster (c, end, short_circuit);
      ଫେରନ୍ତୁ;
    }

  ଯେତେବେଳେ (buffer->idx < end && buffer->successful)
    decompose_current_character (c, short_circuit);
}


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_combining_class (ଅଚଳ hb_glyph_info_t *pa, ଅଚଳ hb_glyph_info_t *pb)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = _hb_glyph_info_get_modified_combining_class (pa);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = _hb_glyph_info_get_modified_combining_class (pb);

  ଫେରନ୍ତୁ a < b ? -1 : a == b ? 0 : +1;
}


ଶୂନ୍ୟ
_hb_ot_shape_normalize (ଅଚଳ hb_ot_shape_plan_t *plan,
			hb_buffer_t *buffer,
			hb_font_t *font)
{
  ଯଦି (unlikely (!buffer->len)) ଫେରନ୍ତୁ;

  _hb_buffer_assert_unicode_vars (buffer);

  hb_ot_shape_normalization_mode_t mode = plan->shaper->normalization_preference;
  ଯଦି (mode == HB_OT_SHAPE_NORMALIZATION_MODE_AUTO)
  {
    ଯଦି (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      mode = HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS;
    ଅନ୍ୟଥା
      mode = HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS;
  }

  hb_ot_shape_normalize_context_t c = {
    plan,
    buffer,
    font,
    buffer->unicode,
    plan->shaper->decompose ? plan->shaper->decompose : hb_ot_shape_normalize_context_t::decompose_unicode,
    plan->shaper->compose   ? plan->shaper->compose   : hb_ot_shape_normalize_context_t::compose_unicode
  };
  c.override_decompose_and_compose (plan->shaper->decompose, plan->shaper->compose);

  ତାର୍କିକ always_short_circuit = mode == HB_OT_SHAPE_NORMALIZATION_MODE_NONE;
  ତାର୍କିକ might_short_circuit = always_short_circuit ||
			     (mode != HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED &&
			      mode != HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  ତାର୍କିକ all_simple = ସତ୍ୟ;
  ଯଦି (buffer->message (font, "start decompose"))
  {
    buffer->clear_output ();
    ଗଣନା_2 = buffer->len;
    buffer->idx = 0;
    କର
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end;
      ପାଇଁ (end = buffer->idx + 1; end < ଗଣନା_2; end++)
	ଯଦି (_hb_glyph_info_is_unicode_mark (&buffer->info[end]))
	  ଭାଙ୍ଗ;

      ଯଦି (end < ଗଣନା_2)
	end--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      ଯଦି (might_short_circuit)
      {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା done = font->get_nominal_glyphs (end - buffer->idx,
						      &buffer->cur().codepoint,
						      ଆକାର (buffer->info[0]),
						      &buffer->cur().normalizer_glyph_index(),
						      ଆକାର (buffer->info[0]));
	ଯଦି (unlikely (!buffer->next_glyphs (done))) ଭାଙ୍ଗ;
      }
      ଯେତେବେଳେ (buffer->idx < end && buffer->successful)
	decompose_current_character (&c, might_short_circuit);

      ଯଦି (buffer->idx == ଗଣନା_2 || !buffer->successful)
	ଭାଙ୍ଗ;

      all_simple = ମିଥ୍ୟା;

      /* Find all the marks now. */
      ପାଇଁ (end = buffer->idx + 1; end < ଗଣନା_2; end++)
	ଯଦି (!_hb_glyph_info_is_unicode_mark(&buffer->info[end]))
	  ଭାଙ୍ଗ;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, end, always_short_circuit);
    }
    ଯେତେବେଳେ (buffer->idx < ଗଣନା_2 && buffer->successful);
    buffer->ସମକାଳ ();
    (ଶୂନ୍ୟ) buffer->message (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  ଯଦି (!all_simple && buffer->message(font, "start reorder"))
  {
    ଗଣନା_2 = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	ଜାରି;

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end;
      ପାଇଁ (end = i + 1; end < ଗଣନା_2; end++)
	ଯଦି (_hb_glyph_info_get_modified_combining_class (&info[end]) == 0)
	  ଭାଙ୍ଗ;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      ଯଦି (end - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = end;
	ଜାରି;
      }

      buffer->sort (i, end, compare_combining_class);

      ଯଦି (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, buffer, i, end);

      i = end;
    }
    (ଶୂନ୍ୟ) buffer->message(font, "end reorder");
  }
  ଯଦି (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    unsigned ଗଣନା_2 = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i + 1 < ଗଣନା_2; i++)
      ଯଦି (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  ଯଦି (!all_simple &&
      buffer->successful &&
      (mode == HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS ||
       mode == HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      buffer->message (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    buffer->clear_output ();
    ଗଣନା_2 = buffer->len;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା starter = 0;
    (ଶୂନ୍ୟ) buffer->next_glyph ();
    ଯେତେବେଳେ (buffer->idx < ଗଣନା_2 /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      ଯଦି (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&buffer->cur()))
      {
	ଯଦି (/* If there's anything between the starter and this char, they should have CCC
	     * smaller than this character's. */
	    (starter == buffer->out_len - 1 ||
	     info_cc (buffer->prev()) < info_cc (buffer->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       buffer->out_info[starter].codepoint,
		       buffer->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  ଯଦି (unlikely (!buffer->next_glyph ())) ଭାଙ୍ଗ; /* Copy to out-buffer. */
	  buffer->merge_out_clusters (starter, buffer->out_len);
	  buffer->out_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  buffer->out_info[starter].codepoint = composed;
	  buffer->out_info[starter].normalizer_glyph_index() = glyph;
	  _hb_glyph_info_set_unicode_props (&buffer->out_info[starter], buffer);

	  ଜାରି;
	}
      }

      /* Blocked, or doesn't compose. */
      ଯଦି (unlikely (!buffer->next_glyph ())) ଭାଙ୍ଗ;

      ଯଦି (info_cc (buffer->prev()) == 0)
	starter = buffer->out_len - 1;
    }
    buffer->ସମକାଳ ();
    (ଶୂନ୍ୟ) buffer->message (font, "end compose");
  }
}


#endif
