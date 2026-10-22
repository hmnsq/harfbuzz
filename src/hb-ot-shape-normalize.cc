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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE

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
سيٽ_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (فراغ) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_اشاريو());
}

ساكن مضمن فراغ
output_char (hb_بفر_t *بفر, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  بفر->cur().normalizer_glyph_اشاريو() = glyph;
  (فراغ) بفر->output_glyph (unichar);
  hb_glyph_info_سيٽ_unicode_props (&بفر->prev(), بفر);
}

ساكن مضمن فراغ
ايندڙ_char (hb_بفر_t *بفر, hb_codepoint_t glyph)
{
  بفر->cur().normalizer_glyph_اشاريو() = glyph;
  (فراغ) بفر->ايندڙ_glyph ();
}

ساكن مضمن فراغ
skip_char (hb_بفر_t *بفر)
{
  بفر->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
ساكن مضمن بنانشان صحيح
decompose (ثابت hb_ot_shape_normalize_context_t *c, منطقي سڀ_کان_ٿورو, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_font_t * ثابت font = c->font;

  إذا (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    أرجع 0;

  منطقي has_a = (منطقي) font->get_nominal_glyph (a, &a_glyph);
  إذا (سڀ_کان_ٿورو && has_a) {
    /* Output a and b */
    output_char (بفر, a, a_glyph);
    إذا (likely (b)) {
      output_char (بفر, b, b_glyph);
      أرجع 2;
    }
    أرجع 1;
  }

  إذا (بنانشان ret = decompose (c, سڀ_کان_ٿورو, a)) {
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
decompose_current_character (ثابت hb_ot_shape_normalize_context_t *c, منطقي سڀ_کان_ٿورو)
{
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_codepoint_t u = بفر->cur().codepoint;
  hb_codepoint_t glyph = 0;

  إذا (سڀ_کان_ٿورو && c->font->get_nominal_glyph (u, &glyph, بفر->not_مليو))
  {
    ايندڙ_char (بفر, glyph);
    أرجع;
  }

  إذا (decompose (c, سڀ_کان_ٿورو, u))
  {
    skip_char (بفر);
    أرجع;
  }

  إذا (!سڀ_کان_ٿورو && c->font->get_nominal_glyph (u, &glyph, بفر->not_مليو))
  {
    ايندڙ_char (بفر, glyph);
    أرجع;
  }

  إذا (_hb_glyph_info_is_unicode_space (&بفر->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = بفر->unicode->space_fallback_type (u);
    إذا (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = بفر->invisible)))
    {
      hb_glyph_info_سيٽ_unicode_space_fallback_type (&بفر->cur(), space_type);
      ايندڙ_char (بفر, space_glyph);
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
      ايندڙ_char (بفر, other_glyph);
      أرجع;
    }
  }

  ايندڙ_char (بفر, glyph); /* glyph is initialized in earlier branches. */
}

ساكن مضمن فراغ
handle_variation_selector_cluster (ثابت hb_ot_shape_normalize_context_t *c,
				   بنانشان صحيح پڇاڙي,
				   منطقي short_circuit HB_اڻ_استعمال_ٿيل)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_بفر_t * ثابت بفر = c->بفر;
  hb_font_t * ثابت font = c->font;
  لكل (; بفر->idx < پڇاڙي - 1 && بفر->successful;) {
    إذا (unlikely (بفر->unicode->is_variation_selector (بفر->cur(+1).codepoint))) {
      إذا (font->get_variation_glyph (بفر->cur().codepoint, بفر->cur(+1).codepoint, &بفر->cur().normalizer_glyph_اشاريو()))
      {
	hb_codepoint_t unicode = بفر->cur().codepoint;
	(فراغ) بفر->replace_glyphs (2, 1, &unicode);
      }
      وإلا
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	سيٽ_glyph (بفر->cur(), font);
	(فراغ) بفر->ايندڙ_glyph ();

        بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	hb_glyph_info_سيٽ_variation_selector (&بفر->cur(), سچ);
	إذا (بفر->not_مليو_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&بفر->cur());

	سيٽ_glyph (بفر->cur(), font);
	(فراغ) بفر->ايندڙ_glyph ();
      }
      /* Skip any further variation selectors. */
      طالما (بفر->idx < پڇاڙي &&
	     بفر->successful &&
	     unlikely (بفر->unicode->is_variation_selector (بفر->cur().codepoint)))
      {
	سيٽ_glyph (بفر->cur(), font);
	(فراغ) بفر->ايندڙ_glyph ();
      }
    }
    وإلا
    {
      سيٽ_glyph (بفر->cur(), font);
      (فراغ) بفر->ايندڙ_glyph ();
    }
  }
  إذا (likely (بفر->idx < پڇاڙي))
  {
    سيٽ_glyph (بفر->cur(), font);
    (فراغ) بفر->ايندڙ_glyph ();
  }
}

ساكن مضمن فراغ
decompose_multi_char_cluster (ثابت hb_ot_shape_normalize_context_t *c, بنانشان صحيح پڇاڙي, منطقي short_circuit)
{
  hb_بفر_t * ثابت بفر = c->بفر;
  لكل (بنانشان صحيح i = بفر->idx; i < پڇاڙي && بفر->successful; i++)
    إذا (unlikely (بفر->unicode->is_variation_selector (بفر->info[i].codepoint))) {
      handle_variation_selector_cluster (c, پڇاڙي, short_circuit);
      أرجع;
    }

  طالما (بفر->idx < پڇاڙي && بفر->successful)
    decompose_current_character (c, short_circuit);
}


ساكن صحيح
compare_combining_class (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  بنانشان صحيح a = _hb_glyph_info_get_modified_combining_class (pa);
  بنانشان صحيح b = _hb_glyph_info_get_modified_combining_class (pb);

  أرجع a < b ? -1 : a == b ? 0 : +1;
}


فراغ
_hb_ot_shape_normalize (ثابت hb_ot_shape_plan_t *plan,
			hb_بفر_t *بفر,
			hb_font_t *font)
{
  إذا (unlikely (!بفر->len)) أرجع;

  hb_بفر_assert_unicode_vars (بفر);

  hb_ot_shape_normalization_گهڻائي_t گهڻائي = plan->shaper->normalization_preference;
  إذا (گهڻائي == HB_OT_SHAPE_NORMALIZATION_گهڻائي_AUTO)
  {
    إذا (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      گهڻائي = HB_OT_SHAPE_NORMALIZATION_گهڻائي_COMPOSED_DIACRITICS;
    وإلا
      گهڻائي = HB_OT_SHAPE_NORMALIZATION_گهڻائي_COMPOSED_DIACRITICS;
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

  منطقي always_short_circuit = گهڻائي == HB_OT_SHAPE_NORMALIZATION_گهڻائي_NONE;
  منطقي might_short_circuit = always_short_circuit ||
			     (گهڻائي != HB_OT_SHAPE_NORMALIZATION_گهڻائي_DECOMPOSED &&
			      گهڻائي != HB_OT_SHAPE_NORMALIZATION_گهڻائي_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  بنانشان صحيح ڳڻپ;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  منطقي all_simple = سچ;
  إذا (بفر->پيغام (font, "start decompose"))
  {
    بفر->clear_output ();
    ڳڻپ = بفر->len;
    بفر->idx = 0;
    افعل
    {
      بنانشان صحيح پڇاڙي;
      لكل (پڇاڙي = بفر->idx + 1; پڇاڙي < ڳڻپ; پڇاڙي++)
	إذا (_hb_glyph_info_is_unicode_mark (&بفر->info[پڇاڙي]))
	  اكسر;

      إذا (پڇاڙي < ڳڻپ)
	پڇاڙي--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      إذا (might_short_circuit)
      {
	بنانشان صحيح پورو = font->get_nominal_glyphs (پڇاڙي - بفر->idx,
						      &بفر->cur().codepoint,
						      حجم (بفر->info[0]),
						      &بفر->cur().normalizer_glyph_اشاريو(),
						      حجم (بفر->info[0]));
	إذا (unlikely (!بفر->ايندڙ_glyphs (پورو))) اكسر;
      }
      طالما (بفر->idx < پڇاڙي && بفر->successful)
	decompose_current_character (&c, might_short_circuit);

      إذا (بفر->idx == ڳڻپ || !بفر->successful)
	اكسر;

      all_simple = خطأ;

      /* Find all the marks now. */
      لكل (پڇاڙي = بفر->idx + 1; پڇاڙي < ڳڻپ; پڇاڙي++)
	إذا (!_hb_glyph_info_is_unicode_mark(&بفر->info[پڇاڙي]))
	  اكسر;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, پڇاڙي, always_short_circuit);
    }
    طالما (بفر->idx < ڳڻپ && بفر->successful);
    بفر->متزامن ();
    (فراغ) بفر->پيغام (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  إذا (!all_simple && بفر->پيغام(font, "start reorder"))
  {
    ڳڻپ = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	استمر;

      بنانشان صحيح پڇاڙي;
      لكل (پڇاڙي = i + 1; پڇاڙي < ڳڻپ; پڇاڙي++)
	إذا (_hb_glyph_info_get_modified_combining_class (&info[پڇاڙي]) == 0)
	  اكسر;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      إذا (پڇاڙي - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = پڇاڙي;
	استمر;
      }

      بفر->sort (i, پڇاڙي, compare_combining_class);

      إذا (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, بفر, i, پڇاڙي);

      i = پڇاڙي;
    }
    (فراغ) بفر->پيغام(font, "end reorder");
  }
  إذا (بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    بنانشان ڳڻپ = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    لكل (بنانشان صحيح i = 1; i + 1 < ڳڻپ; i++)
      إذا (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  إذا (!all_simple &&
      بفر->successful &&
      (گهڻائي == HB_OT_SHAPE_NORMALIZATION_گهڻائي_COMPOSED_DIACRITICS ||
       گهڻائي == HB_OT_SHAPE_NORMALIZATION_گهڻائي_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      بفر->پيغام (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    بفر->clear_output ();
    ڳڻپ = بفر->len;
    بنانشان صحيح starter = 0;
    (فراغ) بفر->ايندڙ_glyph ();
    طالما (بفر->idx < ڳڻپ /* No need for: && buffer->successful */)
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
	    (starter == بفر->ٻاهريون_len - 1 ||
	     info_cc (بفر->prev()) < info_cc (بفر->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       بفر->ٻاهريون_info[starter].codepoint,
		       بفر->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  إذا (unlikely (!بفر->ايندڙ_glyph ())) اكسر; /* Copy to out-buffer. */
	  بفر->ملايو_ٻاهريون_clusters (starter, بفر->ٻاهريون_len);
	  بفر->ٻاهريون_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  بفر->ٻاهريون_info[starter].codepoint = composed;
	  بفر->ٻاهريون_info[starter].normalizer_glyph_اشاريو() = glyph;
	  hb_glyph_info_سيٽ_unicode_props (&بفر->ٻاهريون_info[starter], بفر);

	  استمر;
	}
      }

      /* Blocked, or doesn't compose. */
      إذا (unlikely (!بفر->ايندڙ_glyph ())) اكسر;

      إذا (info_cc (بفر->prev()) == 0)
	starter = بفر->ٻاهريون_len - 1;
    }
    بفر->متزامن ();
    (فراغ) بفر->پيغام (font, "end compose");
  }
}


#جيڪڏهن_پورو
