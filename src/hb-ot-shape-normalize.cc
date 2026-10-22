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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাব "hb-ot-shape-normalize.hh"
#অন্তর্ভাব "hb-ot-shaper.hh"
#অন্তর্ভাব "hb-ot-shape.hh"


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


স্থির অন্তঃরেখ শূন্য
set_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (শূন্য) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_সূচক());
}

স্থির অন্তঃরেখ শূন্য
output_char (hb_বাফার_t *বাফার, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  বাফার->cur().normalizer_glyph_সূচক() = glyph;
  (শূন্য) বাফার->output_glyph (unichar);
  _hb_glyph_info_set_unicode_props (&বাফার->prev(), বাফার);
}

স্থির অন্তঃরেখ শূন্য
পরবর্তী_char (hb_বাফার_t *বাফার, hb_codepoint_t glyph)
{
  বাফার->cur().normalizer_glyph_সূচক() = glyph;
  (শূন্য) বাফার->পরবর্তী_glyph ();
}

স্থির অন্তঃরেখ শূন্য
skip_char (hb_বাফার_t *বাফার)
{
  বাফার->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
decompose (ধ্রুবক hb_ot_shape_normalize_context_t *c, যৌক্তিক shortest, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_বাফার_t * ধ্রুবক বাফার = c->বাফার;
  hb_font_t * ধ্রুবক font = c->font;

  যদি (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    ফেরত 0;

  যৌক্তিক has_a = (যৌক্তিক) font->get_nominal_glyph (a, &a_glyph);
  যদি (shortest && has_a) {
    /* Output a and b */
    output_char (বাফার, a, a_glyph);
    যদি (likely (b)) {
      output_char (বাফার, b, b_glyph);
      ফেরত 2;
    }
    ফেরত 1;
  }

  যদি (unsigned ret = decompose (c, shortest, a)) {
    যদি (b) {
      output_char (বাফার, b, b_glyph);
      ফেরত ret + 1;
    }
    ফেরত ret;
  }

  যদি (has_a) {
    output_char (বাফার, a, a_glyph);
    যদি (likely (b)) {
      output_char (বাফার, b, b_glyph);
      ফেরত 2;
    }
    ফেরত 1;
  }

  ফেরত 0;
}

স্থির অন্তঃরেখ শূন্য
decompose_current_character (ধ্রুবক hb_ot_shape_normalize_context_t *c, যৌক্তিক shortest)
{
  hb_বাফার_t * ধ্রুবক বাফার = c->বাফার;
  hb_codepoint_t u = বাফার->cur().codepoint;
  hb_codepoint_t glyph = 0;

  যদি (shortest && c->font->get_nominal_glyph (u, &glyph, বাফার->not_found))
  {
    পরবর্তী_char (বাফার, glyph);
    ফেরত;
  }

  যদি (decompose (c, shortest, u))
  {
    skip_char (বাফার);
    ফেরত;
  }

  যদি (!shortest && c->font->get_nominal_glyph (u, &glyph, বাফার->not_found))
  {
    পরবর্তী_char (বাফার, glyph);
    ফেরত;
  }

  যদি (_hb_glyph_info_is_unicode_space (&বাফার->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = বাফার->unicode->space_fallback_type (u);
    যদি (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = বাফার->invisible)))
    {
      _hb_glyph_info_set_unicode_space_fallback_type (&বাফার->cur(), space_type);
      পরবর্তী_char (বাফার, space_glyph);
      বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
      ফেরত;
    }
  }

  যদি (u == 0x2011u)
  {
    /* U+2011 is the only sensible character that is a no-break version of another character
     * and not a space.  The space ones are handled already.  Handle this lone one. */
    hb_codepoint_t other_glyph;
    যদি (c->font->get_nominal_glyph (0x2010u, &other_glyph))
    {
      পরবর্তী_char (বাফার, other_glyph);
      ফেরত;
    }
  }

  পরবর্তী_char (বাফার, glyph); /* glyph is initialized in earlier branches. */
}

স্থির অন্তঃরেখ শূন্য
handle_variation_selector_cluster (ধ্রুবক hb_ot_shape_normalize_context_t *c,
				   unsigned পূর্ণসংখ্যা end,
				   যৌক্তিক short_circuit HB_UNUSED)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_বাফার_t * ধ্রুবক বাফার = c->বাফার;
  hb_font_t * ধ্রুবক font = c->font;
  জন্য (; বাফার->idx < end - 1 && বাফার->successful;) {
    যদি (unlikely (বাফার->unicode->is_variation_selector (বাফার->cur(+1).codepoint))) {
      যদি (font->get_variation_glyph (বাফার->cur().codepoint, বাফার->cur(+1).codepoint, &বাফার->cur().normalizer_glyph_সূচক()))
      {
	hb_codepoint_t unicode = বাফার->cur().codepoint;
	(শূন্য) বাফার->replace_glyphs (2, 1, &unicode);
      }
      অন্যথা
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	set_glyph (বাফার->cur(), font);
	(শূন্য) বাফার->পরবর্তী_glyph ();

        বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	_hb_glyph_info_set_variation_selector (&বাফার->cur(), সত্য);
	যদি (বাফার->not_found_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&বাফার->cur());

	set_glyph (বাফার->cur(), font);
	(শূন্য) বাফার->পরবর্তী_glyph ();
      }
      /* Skip any further variation selectors. */
      যখন (বাফার->idx < end &&
	     বাফার->successful &&
	     unlikely (বাফার->unicode->is_variation_selector (বাফার->cur().codepoint)))
      {
	set_glyph (বাফার->cur(), font);
	(শূন্য) বাফার->পরবর্তী_glyph ();
      }
    }
    অন্যথা
    {
      set_glyph (বাফার->cur(), font);
      (শূন্য) বাফার->পরবর্তী_glyph ();
    }
  }
  যদি (likely (বাফার->idx < end))
  {
    set_glyph (বাফার->cur(), font);
    (শূন্য) বাফার->পরবর্তী_glyph ();
  }
}

স্থির অন্তঃরেখ শূন্য
decompose_multi_char_cluster (ধ্রুবক hb_ot_shape_normalize_context_t *c, unsigned পূর্ণসংখ্যা end, যৌক্তিক short_circuit)
{
  hb_বাফার_t * ধ্রুবক বাফার = c->বাফার;
  জন্য (unsigned পূর্ণসংখ্যা i = বাফার->idx; i < end && বাফার->successful; i++)
    যদি (unlikely (বাফার->unicode->is_variation_selector (বাফার->info[i].codepoint))) {
      handle_variation_selector_cluster (c, end, short_circuit);
      ফেরত;
    }

  যখন (বাফার->idx < end && বাফার->successful)
    decompose_current_character (c, short_circuit);
}


স্থির পূর্ণসংখ্যা
compare_combining_class (ধ্রুবক hb_glyph_info_t *pa, ধ্রুবক hb_glyph_info_t *pb)
{
  unsigned পূর্ণসংখ্যা a = _hb_glyph_info_get_modified_combining_class (pa);
  unsigned পূর্ণসংখ্যা b = _hb_glyph_info_get_modified_combining_class (pb);

  ফেরত a < b ? -1 : a == b ? 0 : +1;
}


শূন্য
_hb_ot_shape_normalize (ধ্রুবক hb_ot_shape_plan_t *plan,
			hb_বাফার_t *বাফার,
			hb_font_t *font)
{
  যদি (unlikely (!বাফার->len)) ফেরত;

  hb_বাফার_assert_unicode_vars (বাফার);

  hb_ot_shape_normalization_mode_t mode = plan->shaper->normalization_preference;
  যদি (mode == HB_OT_SHAPE_NORMALIZATION_MODE_AUTO)
  {
    যদি (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      mode = HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS;
    অন্যথা
      mode = HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS;
  }

  hb_ot_shape_normalize_context_t c = {
    plan,
    বাফার,
    font,
    বাফার->unicode,
    plan->shaper->decompose ? plan->shaper->decompose : hb_ot_shape_normalize_context_t::decompose_unicode,
    plan->shaper->compose   ? plan->shaper->compose   : hb_ot_shape_normalize_context_t::compose_unicode
  };
  c.override_decompose_and_compose (plan->shaper->decompose, plan->shaper->compose);

  যৌক্তিক always_short_circuit = mode == HB_OT_SHAPE_NORMALIZATION_MODE_NONE;
  যৌক্তিক might_short_circuit = always_short_circuit ||
			     (mode != HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED &&
			      mode != HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  unsigned পূর্ণসংখ্যা গুনতি;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  যৌক্তিক all_simple = সত্য;
  যদি (বাফার->message (font, "start decompose"))
  {
    বাফার->clear_output ();
    গুনতি = বাফার->len;
    বাফার->idx = 0;
    করো
    {
      unsigned পূর্ণসংখ্যা end;
      জন্য (end = বাফার->idx + 1; end < গুনতি; end++)
	যদি (_hb_glyph_info_is_unicode_mark (&বাফার->info[end]))
	  বিরতি;

      যদি (end < গুনতি)
	end--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      যদি (might_short_circuit)
      {
	unsigned পূর্ণসংখ্যা done = font->get_nominal_glyphs (end - বাফার->idx,
						      &বাফার->cur().codepoint,
						      আকার (বাফার->info[0]),
						      &বাফার->cur().normalizer_glyph_সূচক(),
						      আকার (বাফার->info[0]));
	যদি (unlikely (!বাফার->পরবর্তী_glyphs (done))) বিরতি;
      }
      যখন (বাফার->idx < end && বাফার->successful)
	decompose_current_character (&c, might_short_circuit);

      যদি (বাফার->idx == গুনতি || !বাফার->successful)
	বিরতি;

      all_simple = মিথ্যা;

      /* Find all the marks now. */
      জন্য (end = বাফার->idx + 1; end < গুনতি; end++)
	যদি (!_hb_glyph_info_is_unicode_mark(&বাফার->info[end]))
	  বিরতি;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, end, always_short_circuit);
    }
    যখন (বাফার->idx < গুনতি && বাফার->successful);
    বাফার->সমলয় ();
    (শূন্য) বাফার->message (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  যদি (!all_simple && বাফার->message(font, "start reorder"))
  {
    গুনতি = বাফার->len;
    hb_glyph_info_t *info = বাফার->info;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	চালু;

      unsigned পূর্ণসংখ্যা end;
      জন্য (end = i + 1; end < গুনতি; end++)
	যদি (_hb_glyph_info_get_modified_combining_class (&info[end]) == 0)
	  বিরতি;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      যদি (end - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = end;
	চালু;
      }

      বাফার->sort (i, end, compare_combining_class);

      যদি (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, বাফার, i, end);

      i = end;
    }
    (শূন্য) বাফার->message(font, "end reorder");
  }
  যদি (বাফার->scratch_flags & HB_বাফার_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    unsigned গুনতি = বাফার->len;
    hb_glyph_info_t *info = বাফার->info;
    জন্য (unsigned পূর্ণসংখ্যা i = 1; i + 1 < গুনতি; i++)
      যদি (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  যদি (!all_simple &&
      বাফার->successful &&
      (mode == HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS ||
       mode == HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      বাফার->message (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    বাফার->clear_output ();
    গুনতি = বাফার->len;
    unsigned পূর্ণসংখ্যা starter = 0;
    (শূন্য) বাফার->পরবর্তী_glyph ();
    যখন (বাফার->idx < গুনতি /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      যদি (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&বাফার->cur()))
      {
	যদি (/* If there's anything between the starter and this char, they should have CCC
	     * smaller than this character's. */
	    (starter == বাফার->out_len - 1 ||
	     info_cc (বাফার->prev()) < info_cc (বাফার->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       বাফার->out_info[starter].codepoint,
		       বাফার->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  যদি (unlikely (!বাফার->পরবর্তী_glyph ())) বিরতি; /* Copy to out-buffer. */
	  বাফার->merge_out_clusters (starter, বাফার->out_len);
	  বাফার->out_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  বাফার->out_info[starter].codepoint = composed;
	  বাফার->out_info[starter].normalizer_glyph_সূচক() = glyph;
	  _hb_glyph_info_set_unicode_props (&বাফার->out_info[starter], বাফার);

	  চালু;
	}
      }

      /* Blocked, or doesn't compose. */
      যদি (unlikely (!বাফার->পরবর্তী_glyph ())) বিরতি;

      যদি (info_cc (বাফার->prev()) == 0)
	starter = বাফার->out_len - 1;
    }
    বাফার->সমলয় ();
    (শূন্য) বাফার->message (font, "end compose");
  }
}


#endif
