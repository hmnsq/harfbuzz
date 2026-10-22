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

#ضمن "hb.hh"

#که_نه_تعریف_شوی HB_NO_OT_SHAPE

#ضمن "hb-ot-shape-normalize.hh"
#ضمن "hb-ot-shaper.hh"
#ضمن "hb-ot-shape.hh"


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


ساكن مضمن فراغ
سېټ_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (فراغ) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_شاخص());
}

ساكن مضمن فراغ
output_char (hb_بفر_t *بفر, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  بفر->cur().normalizer_glyph_شاخص() = glyph;
  (فراغ) بفر->output_glyph (unichar);
  hb_glyph_info_سېټ_unicode_props (&بفر->prev(), بفر);
}

ساكن مضمن فراغ
بل_char (hb_بفر_t *بفر, hb_codepoint_t glyph)
{
  بفر->cur().normalizer_glyph_شاخص() = glyph;
  (فراغ) بفر->بل_glyph ();
}

ساكن مضمن فراغ
skip_char (hb_بفر_t *بفر)
{
  بفر->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
ساكن مضمن بینښه صحيح
decompose (ثابت hb_ot_shape_normalize_context_t *c, منطقي تر_ټولو_لنډ, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_font_t * ثابت font = c->font;

  إذا (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    أرجع 0;

  منطقي has_a = (منطقي) font->get_nominal_glyph (a, &a_glyph);
  إذا (تر_ټولو_لنډ && has_a) {
    /* Output a and b */
    output_char (بفر, a, a_glyph);
    إذا (likely (b)) {
      output_char (بفر, b, b_glyph);
      أرجع 2;
    }
    أرجع 1;
  }

  إذا (بینښه ret = decompose (c, تر_ټولو_لنډ, a)) {
    إذا (b) {
      output_char (بفر, b, b_glyph);
      أرجع ret + 1;
    }
    أرجع ret;
  }

  إذا (has_a) {
    output_char (بفر, a, a_glyph);
    إذا (likely (b)) {
      output_char (بفر, b, b_glyph);
      أرجع 2;
    }
    أرجع 1;
  }

  أرجع 0;
}

ساكن مضمن فراغ
decompose_current_character (ثابت hb_ot_shape_normalize_context_t *c, منطقي تر_ټولو_لنډ)
{
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_codepoint_t u = بفر->cur().codepoint;
  hb_codepoint_t glyph = 0;

  إذا (تر_ټولو_لنډ && c->font->get_nominal_glyph (u, &glyph, بفر->not_وموندل))
  {
    بل_char (بفر, glyph);
    أرجع;
  }

  إذا (decompose (c, تر_ټولو_لنډ, u))
  {
    skip_char (بفر);
    أرجع;
  }

  إذا (!تر_ټولو_لنډ && c->font->get_nominal_glyph (u, &glyph, بفر->not_وموندل))
  {
    بل_char (بفر, glyph);
    أرجع;
  }

  إذا (_hb_glyph_info_is_unicode_space (&بفر->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = بفر->unicode->space_fallback_type (u);
    إذا (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = بفر->invisible)))
    {
      hb_glyph_info_سېټ_unicode_space_fallback_type (&بفر->cur(), space_type);
      بل_char (بفر, space_glyph);
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
      أرجع;
    }
  }

  إذا (u == 0x2011u)
  {
    /* U+2011 is the only sensible character that is a no-break version of another character
     * and not a space.  The space ones are handled already.  Handle this lone one. */
    hb_codepoint_t other_glyph;
    إذا (c->font->get_nominal_glyph (0x2010u, &other_glyph))
    {
      بل_char (بفر, other_glyph);
      أرجع;
    }
  }

  بل_char (بفر, glyph); /* glyph is initialized in earlier branches. */
}

ساكن مضمن فراغ
handle_variation_selector_cluster (ثابت hb_ot_shape_normalize_context_t *c,
				   بینښه صحيح پای_ټکی,
				   منطقي short_circuit HB_ناکارول_شوی)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_font_t * ثابت font = c->font;
  لكل (; بفر->idx < پای_ټکی - 1 && بفر->successful;) {
    إذا (unlikely (بفر->unicode->is_variation_selector (بفر->cur(+1).codepoint))) {
      إذا (font->get_variation_glyph (بفر->cur().codepoint, بفر->cur(+1).codepoint, &بفر->cur().normalizer_glyph_شاخص()))
      {
	hb_codepoint_t unicode = بفر->cur().codepoint;
	(فراغ) بفر->replace_glyphs (2, 1, &unicode);
      }
      وإلا
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	سېټ_glyph (بفر->cur(), font);
	(فراغ) بفر->بل_glyph ();

        بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	hb_glyph_info_سېټ_variation_selector (&بفر->cur(), ریښتیا);
	إذا (بفر->not_وموندل_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&بفر->cur());

	سېټ_glyph (بفر->cur(), font);
	(فراغ) بفر->بل_glyph ();
      }
      /* Skip any further variation selectors. */
      طالما (بفر->idx < پای_ټکی &&
	     بفر->successful &&
	     unlikely (بفر->unicode->is_variation_selector (بفر->cur().codepoint)))
      {
	سېټ_glyph (بفر->cur(), font);
	(فراغ) بفر->بل_glyph ();
      }
    }
    وإلا
    {
      سېټ_glyph (بفر->cur(), font);
      (فراغ) بفر->بل_glyph ();
    }
  }
  إذا (likely (بفر->idx < پای_ټکی))
  {
    سېټ_glyph (بفر->cur(), font);
    (فراغ) بفر->بل_glyph ();
  }
}

ساكن مضمن فراغ
decompose_multi_char_cluster (ثابت hb_ot_shape_normalize_context_t *c, بینښه صحيح پای_ټکی, منطقي short_circuit)
{
  hb_بفر_t * ثابت بفر = c->بفر;
  لكل (بینښه صحيح i = بفر->idx; i < پای_ټکی && بفر->successful; i++)
    إذا (unlikely (بفر->unicode->is_variation_selector (بفر->info[i].codepoint))) {
      handle_variation_selector_cluster (c, پای_ټکی, short_circuit);
      أرجع;
    }

  طالما (بفر->idx < پای_ټکی && بفر->successful)
    decompose_current_character (c, short_circuit);
}


ساكن صحيح
compare_combining_class (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  بینښه صحيح a = _hb_glyph_info_get_modified_combining_class (pa);
  بینښه صحيح b = _hb_glyph_info_get_modified_combining_class (pb);

  أرجع a < b ? -1 : a == b ? 0 : +1;
}


فراغ
_hb_ot_shape_normalize (ثابت hb_ot_shape_plan_t *plan,
			hb_بفر_t *بفر,
			hb_font_t *font)
{
  إذا (unlikely (!بفر->len)) أرجع;

  hb_بفر_assert_unicode_vars (بفر);

  hb_ot_shape_normalization_ډېرپېښ_t ډېرپېښ = plan->shaper->normalization_preference;
  إذا (ډېرپېښ == HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_AUTO)
  {
    إذا (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      ډېرپېښ = HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_COMPOSED_DIACRITICS;
    وإلا
      ډېرپېښ = HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_COMPOSED_DIACRITICS;
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

  منطقي always_short_circuit = ډېرپېښ == HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_NONE;
  منطقي might_short_circuit = always_short_circuit ||
			     (ډېرپېښ != HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_DECOMPOSED &&
			      ډېرپېښ != HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  بینښه صحيح شمېر;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  منطقي all_simple = ریښتیا;
  إذا (بفر->پیغام (font, "start decompose"))
  {
    بفر->clear_output ();
    شمېر = بفر->len;
    بفر->idx = 0;
    افعل
    {
      بینښه صحيح پای_ټکی;
      لكل (پای_ټکی = بفر->idx + 1; پای_ټکی < شمېر; پای_ټکی++)
	إذا (_hb_glyph_info_is_unicode_mark (&بفر->info[پای_ټکی]))
	  اكسر;

      إذا (پای_ټکی < شمېر)
	پای_ټکی--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      إذا (might_short_circuit)
      {
	بینښه صحيح بشپړ = font->get_nominal_glyphs (پای_ټکی - بفر->idx,
						      &بفر->cur().codepoint,
						      حجم (بفر->info[0]),
						      &بفر->cur().normalizer_glyph_شاخص(),
						      حجم (بفر->info[0]));
	إذا (unlikely (!بفر->بل_glyphs (بشپړ))) اكسر;
      }
      طالما (بفر->idx < پای_ټکی && بفر->successful)
	decompose_current_character (&c, might_short_circuit);

      إذا (بفر->idx == شمېر || !بفر->successful)
	اكسر;

      all_simple = خطأ;

      /* Find all the marks now. */
      لكل (پای_ټکی = بفر->idx + 1; پای_ټکی < شمېر; پای_ټکی++)
	إذا (!_hb_glyph_info_is_unicode_mark(&بفر->info[پای_ټکی]))
	  اكسر;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, پای_ټکی, always_short_circuit);
    }
    طالما (بفر->idx < شمېر && بفر->successful);
    بفر->متزامن ();
    (فراغ) بفر->پیغام (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  إذا (!all_simple && بفر->پیغام(font, "start reorder"))
  {
    شمېر = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	استمر;

      بینښه صحيح پای_ټکی;
      لكل (پای_ټکی = i + 1; پای_ټکی < شمېر; پای_ټکی++)
	إذا (_hb_glyph_info_get_modified_combining_class (&info[پای_ټکی]) == 0)
	  اكسر;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      إذا (پای_ټکی - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = پای_ټکی;
	استمر;
      }

      بفر->sort (i, پای_ټکی, compare_combining_class);

      إذا (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, بفر, i, پای_ټکی);

      i = پای_ټکی;
    }
    (فراغ) بفر->پیغام(font, "end reorder");
  }
  إذا (بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    بینښه شمېر = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    لكل (بینښه صحيح i = 1; i + 1 < شمېر; i++)
      إذا (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  إذا (!all_simple &&
      بفر->successful &&
      (ډېرپېښ == HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_COMPOSED_DIACRITICS ||
       ډېرپېښ == HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      بفر->پیغام (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    بفر->clear_output ();
    شمېر = بفر->len;
    بینښه صحيح starter = 0;
    (فراغ) بفر->بل_glyph ();
    طالما (بفر->idx < شمېر /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      إذا (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&بفر->cur()))
      {
	إذا (/* If there's anything between the starter and this char, they should have CCC
	     * smaller than this character's. */
	    (starter == بفر->بهر_len - 1 ||
	     info_cc (بفر->prev()) < info_cc (بفر->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       بفر->بهر_info[starter].codepoint,
		       بفر->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  إذا (unlikely (!بفر->بل_glyph ())) اكسر; /* Copy to out-buffer. */
	  بفر->یوځای_بهر_clusters (starter, بفر->بهر_len);
	  بفر->بهر_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  بفر->بهر_info[starter].codepoint = composed;
	  بفر->بهر_info[starter].normalizer_glyph_شاخص() = glyph;
	  hb_glyph_info_سېټ_unicode_props (&بفر->بهر_info[starter], بفر);

	  استمر;
	}
      }

      /* Blocked, or doesn't compose. */
      إذا (unlikely (!بفر->بل_glyph ())) اكسر;

      إذا (info_cc (بفر->prev()) == 0)
	starter = بفر->بهر_len - 1;
    }
    بفر->متزامن ();
    (فراغ) بفر->پیغام (font, "end compose");
  }
}


#د_که_پای
