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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape-normalize.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"


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


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਸੈੱਟ_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (ਖਾਲੀ) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_ਸੂਚਕਾਂਕ());
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
output_char (hb_ਬਫਰ_t *ਬਫਰ, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  ਬਫਰ->cur().normalizer_glyph_ਸੂਚਕਾਂਕ() = glyph;
  (ਖਾਲੀ) ਬਫਰ->output_glyph (unichar);
  hb_glyph_info_ਸੈੱਟ_unicode_props (&ਬਫਰ->prev(), ਬਫਰ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਅਗਲਾ_char (hb_ਬਫਰ_t *ਬਫਰ, hb_codepoint_t glyph)
{
  ਬਫਰ->cur().normalizer_glyph_ਸੂਚਕਾਂਕ() = glyph;
  (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
skip_char (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਫਰ->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
decompose (ਅਚਲ hb_ot_shape_normalize_context_t *c, ਤਾਰਕਿਕ ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_ਬਫਰ_t * ਅਚਲ ਬਫਰ = c->ਬਫਰ;
  hb_font_t * ਅਚਲ font = c->font;

  ਜੇ (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    ਵਾਪਸ 0;

  ਤਾਰਕਿਕ has_a = (ਤਾਰਕਿਕ) font->get_nominal_glyph (a, &a_glyph);
  ਜੇ (ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ && has_a) {
    /* Output a and b */
    output_char (ਬਫਰ, a, a_glyph);
    ਜੇ (likely (b)) {
      output_char (ਬਫਰ, b, b_glyph);
      ਵਾਪਸ 2;
    }
    ਵਾਪਸ 1;
  }

  ਜੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ret = decompose (c, ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ, a)) {
    ਜੇ (b) {
      output_char (ਬਫਰ, b, b_glyph);
      ਵਾਪਸ ret + 1;
    }
    ਵਾਪਸ ret;
  }

  ਜੇ (has_a) {
    output_char (ਬਫਰ, a, a_glyph);
    ਜੇ (likely (b)) {
      output_char (ਬਫਰ, b, b_glyph);
      ਵਾਪਸ 2;
    }
    ਵਾਪਸ 1;
  }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
decompose_current_character (ਅਚਲ hb_ot_shape_normalize_context_t *c, ਤਾਰਕਿਕ ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ)
{
  hb_ਬਫਰ_t * ਅਚਲ ਬਫਰ = c->ਬਫਰ;
  hb_codepoint_t u = ਬਫਰ->cur().codepoint;
  hb_codepoint_t glyph = 0;

  ਜੇ (ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ && c->font->get_nominal_glyph (u, &glyph, ਬਫਰ->not_ਮਿਲਿਆ))
  {
    ਅਗਲਾ_char (ਬਫਰ, glyph);
    ਵਾਪਸ;
  }

  ਜੇ (decompose (c, ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ, u))
  {
    skip_char (ਬਫਰ);
    ਵਾਪਸ;
  }

  ਜੇ (!ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ && c->font->get_nominal_glyph (u, &glyph, ਬਫਰ->not_ਮਿਲਿਆ))
  {
    ਅਗਲਾ_char (ਬਫਰ, glyph);
    ਵਾਪਸ;
  }

  ਜੇ (_hb_glyph_info_is_unicode_space (&ਬਫਰ->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = ਬਫਰ->unicode->space_fallback_type (u);
    ਜੇ (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = ਬਫਰ->invisible)))
    {
      hb_glyph_info_ਸੈੱਟ_unicode_space_fallback_type (&ਬਫਰ->cur(), space_type);
      ਅਗਲਾ_char (ਬਫਰ, space_glyph);
      ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
      ਵਾਪਸ;
    }
  }

  ਜੇ (u == 0x2011u)
  {
    /* U+2011 is the only sensible character that is a no-break version of another character
     * and not a space.  The space ones are handled already.  Handle this lone one. */
    hb_codepoint_t other_glyph;
    ਜੇ (c->font->get_nominal_glyph (0x2010u, &other_glyph))
    {
      ਅਗਲਾ_char (ਬਫਰ, other_glyph);
      ਵਾਪਸ;
    }
  }

  ਅਗਲਾ_char (ਬਫਰ, glyph); /* glyph is initialized in earlier branches. */
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
handle_variation_selector_cluster (ਅਚਲ hb_ot_shape_normalize_context_t *c,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
				   ਤਾਰਕਿਕ short_circuit HB_ਅਣਵਰਤਿਆ)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_ਬਫਰ_t * ਅਚਲ ਬਫਰ = c->ਬਫਰ;
  hb_font_t * ਅਚਲ font = c->font;
  ਲਈ (; ਬਫਰ->idx < ਸਿਰਾ - 1 && ਬਫਰ->successful;) {
    ਜੇ (unlikely (ਬਫਰ->unicode->is_variation_selector (ਬਫਰ->cur(+1).codepoint))) {
      ਜੇ (font->get_variation_glyph (ਬਫਰ->cur().codepoint, ਬਫਰ->cur(+1).codepoint, &ਬਫਰ->cur().normalizer_glyph_ਸੂਚਕਾਂਕ()))
      {
	hb_codepoint_t unicode = ਬਫਰ->cur().codepoint;
	(ਖਾਲੀ) ਬਫਰ->replace_glyphs (2, 1, &unicode);
      }
      ਨਹੀਂਤਾਂ
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	ਸੈੱਟ_glyph (ਬਫਰ->cur(), font);
	(ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();

        ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	hb_glyph_info_ਸੈੱਟ_variation_selector (&ਬਫਰ->cur(), ਸੱਚ);
	ਜੇ (ਬਫਰ->not_ਮਿਲਿਆ_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&ਬਫਰ->cur());

	ਸੈੱਟ_glyph (ਬਫਰ->cur(), font);
	(ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
      }
      /* Skip any further variation selectors. */
      ਜਦੋਂ (ਬਫਰ->idx < ਸਿਰਾ &&
	     ਬਫਰ->successful &&
	     unlikely (ਬਫਰ->unicode->is_variation_selector (ਬਫਰ->cur().codepoint)))
      {
	ਸੈੱਟ_glyph (ਬਫਰ->cur(), font);
	(ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਸੈੱਟ_glyph (ਬਫਰ->cur(), font);
      (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
    }
  }
  ਜੇ (likely (ਬਫਰ->idx < ਸਿਰਾ))
  {
    ਸੈੱਟ_glyph (ਬਫਰ->cur(), font);
    (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
decompose_multi_char_cluster (ਅਚਲ hb_ot_shape_normalize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ, ਤਾਰਕਿਕ short_circuit)
{
  hb_ਬਫਰ_t * ਅਚਲ ਬਫਰ = c->ਬਫਰ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਬਫਰ->idx; i < ਸਿਰਾ && ਬਫਰ->successful; i++)
    ਜੇ (unlikely (ਬਫਰ->unicode->is_variation_selector (ਬਫਰ->info[i].codepoint))) {
      handle_variation_selector_cluster (c, ਸਿਰਾ, short_circuit);
      ਵਾਪਸ;
    }

  ਜਦੋਂ (ਬਫਰ->idx < ਸਿਰਾ && ਬਫਰ->successful)
    decompose_current_character (c, short_circuit);
}


ਸਥਿਰ ਪੂਰਨ
compare_combining_class (ਅਚਲ hb_glyph_info_t *pa, ਅਚਲ hb_glyph_info_t *pb)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a = _hb_glyph_info_get_modified_combining_class (pa);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b = _hb_glyph_info_get_modified_combining_class (pb);

  ਵਾਪਸ a < b ? -1 : a == b ? 0 : +1;
}


ਖਾਲੀ
_hb_ot_shape_normalize (ਅਚਲ hb_ot_shape_plan_t *plan,
			hb_ਬਫਰ_t *ਬਫਰ,
			hb_font_t *font)
{
  ਜੇ (unlikely (!ਬਫਰ->len)) ਵਾਪਸ;

  hb_ਬਫਰ_assert_unicode_vars (ਬਫਰ);

  hb_ot_shape_normalization_ਬਹੁਲਕ_t ਬਹੁਲਕ = plan->shaper->normalization_preference;
  ਜੇ (ਬਹੁਲਕ == HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_AUTO)
  {
    ਜੇ (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      ਬਹੁਲਕ = HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS;
    ਨਹੀਂਤਾਂ
      ਬਹੁਲਕ = HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS;
  }

  hb_ot_shape_normalize_context_t c = {
    plan,
    ਬਫਰ,
    font,
    ਬਫਰ->unicode,
    plan->shaper->decompose ? plan->shaper->decompose : hb_ot_shape_normalize_context_t::decompose_unicode,
    plan->shaper->compose   ? plan->shaper->compose   : hb_ot_shape_normalize_context_t::compose_unicode
  };
  c.override_decompose_and_compose (plan->shaper->decompose, plan->shaper->compose);

  ਤਾਰਕਿਕ always_short_circuit = ਬਹੁਲਕ == HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_NONE;
  ਤਾਰਕਿਕ might_short_circuit = always_short_circuit ||
			     (ਬਹੁਲਕ != HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_DECOMPOSED &&
			      ਬਹੁਲਕ != HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  ਤਾਰਕਿਕ all_simple = ਸੱਚ;
  ਜੇ (ਬਫਰ->ਸੁਨੇਹਾ (font, "start decompose"))
  {
    ਬਫਰ->clear_output ();
    ਗਿਣਤੀ_2 = ਬਫਰ->len;
    ਬਫਰ->idx = 0;
    ਕਰੋ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ;
      ਲਈ (ਸਿਰਾ = ਬਫਰ->idx + 1; ਸਿਰਾ < ਗਿਣਤੀ_2; ਸਿਰਾ++)
	ਜੇ (_hb_glyph_info_is_unicode_mark (&ਬਫਰ->info[ਸਿਰਾ]))
	  ਰੋਕੋ;

      ਜੇ (ਸਿਰਾ < ਗਿਣਤੀ_2)
	ਸਿਰਾ--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      ਜੇ (might_short_circuit)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪੂਰਾ = font->get_nominal_glyphs (ਸਿਰਾ - ਬਫਰ->idx,
						      &ਬਫਰ->cur().codepoint,
						      ਆਕਾਰ (ਬਫਰ->info[0]),
						      &ਬਫਰ->cur().normalizer_glyph_ਸੂਚਕਾਂਕ(),
						      ਆਕਾਰ (ਬਫਰ->info[0]));
	ਜੇ (unlikely (!ਬਫਰ->ਅਗਲਾ_glyphs (ਪੂਰਾ))) ਰੋਕੋ;
      }
      ਜਦੋਂ (ਬਫਰ->idx < ਸਿਰਾ && ਬਫਰ->successful)
	decompose_current_character (&c, might_short_circuit);

      ਜੇ (ਬਫਰ->idx == ਗਿਣਤੀ_2 || !ਬਫਰ->successful)
	ਰੋਕੋ;

      all_simple = ਝੂਠ;

      /* Find all the marks now. */
      ਲਈ (ਸਿਰਾ = ਬਫਰ->idx + 1; ਸਿਰਾ < ਗਿਣਤੀ_2; ਸਿਰਾ++)
	ਜੇ (!_hb_glyph_info_is_unicode_mark(&ਬਫਰ->info[ਸਿਰਾ]))
	  ਰੋਕੋ;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, ਸਿਰਾ, always_short_circuit);
    }
    ਜਦੋਂ (ਬਫਰ->idx < ਗਿਣਤੀ_2 && ਬਫਰ->successful);
    ਬਫਰ->ਸਮਕਾਲੀ ();
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  ਜੇ (!all_simple && ਬਫਰ->ਸੁਨੇਹਾ(font, "start reorder"))
  {
    ਗਿਣਤੀ_2 = ਬਫਰ->len;
    hb_glyph_info_t *info = ਬਫਰ->info;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	ਜਾਰੀ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ;
      ਲਈ (ਸਿਰਾ = i + 1; ਸਿਰਾ < ਗਿਣਤੀ_2; ਸਿਰਾ++)
	ਜੇ (_hb_glyph_info_get_modified_combining_class (&info[ਸਿਰਾ]) == 0)
	  ਰੋਕੋ;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      ਜੇ (ਸਿਰਾ - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = ਸਿਰਾ;
	ਜਾਰੀ;
      }

      ਬਫਰ->sort (i, ਸਿਰਾ, compare_combining_class);

      ਜੇ (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, ਬਫਰ, i, ਸਿਰਾ);

      i = ਸਿਰਾ;
    }
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ(font, "end reorder");
  }
  ਜੇ (ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਬਫਰ->len;
    hb_glyph_info_t *info = ਬਫਰ->info;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i + 1 < ਗਿਣਤੀ_2; i++)
      ਜੇ (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  ਜੇ (!all_simple &&
      ਬਫਰ->successful &&
      (ਬਹੁਲਕ == HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS ||
       ਬਹੁਲਕ == HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      ਬਫਰ->ਸੁਨੇਹਾ (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    ਬਫਰ->clear_output ();
    ਗਿਣਤੀ_2 = ਬਫਰ->len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ starter = 0;
    (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
    ਜਦੋਂ (ਬਫਰ->idx < ਗਿਣਤੀ_2 /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      ਜੇ (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&ਬਫਰ->cur()))
      {
	ਜੇ (/* If there's anything between the starter and this char, they should have CCC
	     * smaller than this character's. */
	    (starter == ਬਫਰ->ਬਾਹਰਲਾ_len - 1 ||
	     info_cc (ਬਫਰ->prev()) < info_cc (ਬਫਰ->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       ਬਫਰ->ਬਾਹਰਲਾ_info[starter].codepoint,
		       ਬਫਰ->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  ਜੇ (unlikely (!ਬਫਰ->ਅਗਲਾ_glyph ())) ਰੋਕੋ; /* Copy to out-buffer. */
	  ਬਫਰ->ਮਿਲਾਓ_ਬਾਹਰਲਾ_clusters (starter, ਬਫਰ->ਬਾਹਰਲਾ_len);
	  ਬਫਰ->ਬਾਹਰਲਾ_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  ਬਫਰ->ਬਾਹਰਲਾ_info[starter].codepoint = composed;
	  ਬਫਰ->ਬਾਹਰਲਾ_info[starter].normalizer_glyph_ਸੂਚਕਾਂਕ() = glyph;
	  hb_glyph_info_ਸੈੱਟ_unicode_props (&ਬਫਰ->ਬਾਹਰਲਾ_info[starter], ਬਫਰ);

	  ਜਾਰੀ;
	}
      }

      /* Blocked, or doesn't compose. */
      ਜੇ (unlikely (!ਬਫਰ->ਅਗਲਾ_glyph ())) ਰੋਕੋ;

      ਜੇ (info_cc (ਬਫਰ->prev()) == 0)
	starter = ਬਫਰ->ਬਾਹਰਲਾ_len - 1;
    }
    ਬਫਰ->ਸਮਕਾਲੀ ();
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end compose");
  }
}


#ਜੇ_ਅੰਤ
