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

#اگر_تعریف_نشده HB_NO_OT_SHAPE

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
مجموعه_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (فراغ) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_شاخص());
}

ساكن مضمن فراغ
output_char (hb_میانگیر_t *میانگیر, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  میانگیر->cur().normalizer_glyph_شاخص() = glyph;
  (فراغ) میانگیر->output_glyph (unichar);
  hb_glyph_info_مجموعه_unicode_props (&میانگیر->prev(), میانگیر);
}

ساكن مضمن فراغ
بعدی_char (hb_میانگیر_t *میانگیر, hb_codepoint_t glyph)
{
  میانگیر->cur().normalizer_glyph_شاخص() = glyph;
  (فراغ) میانگیر->بعدی_glyph ();
}

ساكن مضمن فراغ
skip_char (hb_میانگیر_t *میانگیر)
{
  میانگیر->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
ساكن مضمن بیعلامت صحيح
decompose (ثابت hb_ot_shape_normalize_context_t *c, منطقي کوتاهترین, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_میانگیر_t * ثابت میانگیر = c->میانگیر;
  hb_font_t * ثابت font = c->font;

  إذا (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    أرجع 0;

  منطقي has_a = (منطقي) font->get_nominal_glyph (a, &a_glyph);
  إذا (کوتاهترین && has_a) {
    /* Output a and b */
    output_char (میانگیر, a, a_glyph);
    إذا (likely (b)) {
      output_char (میانگیر, b, b_glyph);
      أرجع 2;
    }
    أرجع 1;
  }

  إذا (بیعلامت ret = decompose (c, کوتاهترین, a)) {
    إذا (b) {
      output_char (میانگیر, b, b_glyph);
      أرجع ret + 1;
    }
    أرجع ret;
  }

  إذا (has_a) {
    output_char (میانگیر, a, a_glyph);
    إذا (likely (b)) {
      output_char (میانگیر, b, b_glyph);
      أرجع 2;
    }
    أرجع 1;
  }

  أرجع 0;
}

ساكن مضمن فراغ
decompose_current_character (ثابت hb_ot_shape_normalize_context_t *c, منطقي کوتاهترین)
{
  hb_میانگیر_t * ثابت میانگیر = c->میانگیر;
  hb_codepoint_t u = میانگیر->cur().codepoint;
  hb_codepoint_t glyph = 0;

  إذا (کوتاهترین && c->font->get_nominal_glyph (u, &glyph, میانگیر->not_یافت))
  {
    بعدی_char (میانگیر, glyph);
    أرجع;
  }

  إذا (decompose (c, کوتاهترین, u))
  {
    skip_char (میانگیر);
    أرجع;
  }

  إذا (!کوتاهترین && c->font->get_nominal_glyph (u, &glyph, میانگیر->not_یافت))
  {
    بعدی_char (میانگیر, glyph);
    أرجع;
  }

  إذا (_hb_glyph_info_is_unicode_space (&میانگیر->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = میانگیر->unicode->space_fallback_type (u);
    إذا (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = میانگیر->invisible)))
    {
      hb_glyph_info_مجموعه_unicode_space_fallback_type (&میانگیر->cur(), space_type);
      بعدی_char (میانگیر, space_glyph);
      میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
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
      بعدی_char (میانگیر, other_glyph);
      أرجع;
    }
  }

  بعدی_char (میانگیر, glyph); /* glyph is initialized in earlier branches. */
}

ساكن مضمن فراغ
handle_variation_selector_cluster (ثابت hb_ot_shape_normalize_context_t *c,
				   بیعلامت صحيح انتها,
				   منطقي short_circuit HB_استفادهنشده)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_میانگیر_t * ثابت میانگیر = c->میانگیر;
  hb_font_t * ثابت font = c->font;
  لكل (; میانگیر->idx < انتها - 1 && میانگیر->successful;) {
    إذا (unlikely (میانگیر->unicode->is_variation_selector (میانگیر->cur(+1).codepoint))) {
      إذا (font->get_variation_glyph (میانگیر->cur().codepoint, میانگیر->cur(+1).codepoint, &میانگیر->cur().normalizer_glyph_شاخص()))
      {
	hb_codepoint_t unicode = میانگیر->cur().codepoint;
	(فراغ) میانگیر->replace_glyphs (2, 1, &unicode);
      }
      وإلا
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	مجموعه_glyph (میانگیر->cur(), font);
	(فراغ) میانگیر->بعدی_glyph ();

        میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	hb_glyph_info_مجموعه_variation_selector (&میانگیر->cur(), درست);
	إذا (میانگیر->not_یافت_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&میانگیر->cur());

	مجموعه_glyph (میانگیر->cur(), font);
	(فراغ) میانگیر->بعدی_glyph ();
      }
      /* Skip any further variation selectors. */
      طالما (میانگیر->idx < انتها &&
	     میانگیر->successful &&
	     unlikely (میانگیر->unicode->is_variation_selector (میانگیر->cur().codepoint)))
      {
	مجموعه_glyph (میانگیر->cur(), font);
	(فراغ) میانگیر->بعدی_glyph ();
      }
    }
    وإلا
    {
      مجموعه_glyph (میانگیر->cur(), font);
      (فراغ) میانگیر->بعدی_glyph ();
    }
  }
  إذا (likely (میانگیر->idx < انتها))
  {
    مجموعه_glyph (میانگیر->cur(), font);
    (فراغ) میانگیر->بعدی_glyph ();
  }
}

ساكن مضمن فراغ
decompose_multi_char_cluster (ثابت hb_ot_shape_normalize_context_t *c, بیعلامت صحيح انتها, منطقي short_circuit)
{
  hb_میانگیر_t * ثابت میانگیر = c->میانگیر;
  لكل (بیعلامت صحيح i = میانگیر->idx; i < انتها && میانگیر->successful; i++)
    إذا (unlikely (میانگیر->unicode->is_variation_selector (میانگیر->info[i].codepoint))) {
      handle_variation_selector_cluster (c, انتها, short_circuit);
      أرجع;
    }

  طالما (میانگیر->idx < انتها && میانگیر->successful)
    decompose_current_character (c, short_circuit);
}


ساكن صحيح
compare_combining_class (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  بیعلامت صحيح a = _hb_glyph_info_get_modified_combining_class (pa);
  بیعلامت صحيح b = _hb_glyph_info_get_modified_combining_class (pb);

  أرجع a < b ? -1 : a == b ? 0 : +1;
}


فراغ
_hb_ot_shape_normalize (ثابت hb_ot_shape_plan_t *plan,
			hb_میانگیر_t *میانگیر,
			hb_font_t *font)
{
  إذا (unlikely (!میانگیر->len)) أرجع;

  hb_میانگیر_assert_unicode_vars (میانگیر);

  hb_ot_shape_normalization_نما_t نما = plan->shaper->normalization_preference;
  إذا (نما == HB_OT_SHAPE_NORMALIZATION_نما_AUTO)
  {
    إذا (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      نما = HB_OT_SHAPE_NORMALIZATION_نما_COMPOSED_DIACRITICS;
    وإلا
      نما = HB_OT_SHAPE_NORMALIZATION_نما_COMPOSED_DIACRITICS;
  }

  hb_ot_shape_normalize_context_t c = {
    plan,
    میانگیر,
    font,
    میانگیر->unicode,
    plan->shaper->decompose ? plan->shaper->decompose : hb_ot_shape_normalize_context_t::decompose_unicode,
    plan->shaper->compose   ? plan->shaper->compose   : hb_ot_shape_normalize_context_t::compose_unicode
  };
  c.override_decompose_and_compose (plan->shaper->decompose, plan->shaper->compose);

  منطقي always_short_circuit = نما == HB_OT_SHAPE_NORMALIZATION_نما_NONE;
  منطقي might_short_circuit = always_short_circuit ||
			     (نما != HB_OT_SHAPE_NORMALIZATION_نما_DECOMPOSED &&
			      نما != HB_OT_SHAPE_NORMALIZATION_نما_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  بیعلامت صحيح شمار;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  منطقي all_simple = درست;
  إذا (میانگیر->پیام (font, "start decompose"))
  {
    میانگیر->clear_output ();
    شمار = میانگیر->len;
    میانگیر->idx = 0;
    افعل
    {
      بیعلامت صحيح انتها;
      لكل (انتها = میانگیر->idx + 1; انتها < شمار; انتها++)
	إذا (_hb_glyph_info_is_unicode_mark (&میانگیر->info[انتها]))
	  اكسر;

      إذا (انتها < شمار)
	انتها--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      إذا (might_short_circuit)
      {
	بیعلامت صحيح انجامشده = font->get_nominal_glyphs (انتها - میانگیر->idx,
						      &میانگیر->cur().codepoint,
						      حجم (میانگیر->info[0]),
						      &میانگیر->cur().normalizer_glyph_شاخص(),
						      حجم (میانگیر->info[0]));
	إذا (unlikely (!میانگیر->بعدی_glyphs (انجامشده))) اكسر;
      }
      طالما (میانگیر->idx < انتها && میانگیر->successful)
	decompose_current_character (&c, might_short_circuit);

      إذا (میانگیر->idx == شمار || !میانگیر->successful)
	اكسر;

      all_simple = خطأ;

      /* Find all the marks now. */
      لكل (انتها = میانگیر->idx + 1; انتها < شمار; انتها++)
	إذا (!_hb_glyph_info_is_unicode_mark(&میانگیر->info[انتها]))
	  اكسر;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, انتها, always_short_circuit);
    }
    طالما (میانگیر->idx < شمار && میانگیر->successful);
    میانگیر->متزامن ();
    (فراغ) میانگیر->پیام (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  إذا (!all_simple && میانگیر->پیام(font, "start reorder"))
  {
    شمار = میانگیر->len;
    hb_glyph_info_t *info = میانگیر->info;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	استمر;

      بیعلامت صحيح انتها;
      لكل (انتها = i + 1; انتها < شمار; انتها++)
	إذا (_hb_glyph_info_get_modified_combining_class (&info[انتها]) == 0)
	  اكسر;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      إذا (انتها - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = انتها;
	استمر;
      }

      میانگیر->sort (i, انتها, compare_combining_class);

      إذا (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, میانگیر, i, انتها);

      i = انتها;
    }
    (فراغ) میانگیر->پیام(font, "end reorder");
  }
  إذا (میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    بیعلامت شمار = میانگیر->len;
    hb_glyph_info_t *info = میانگیر->info;
    لكل (بیعلامت صحيح i = 1; i + 1 < شمار; i++)
      إذا (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  إذا (!all_simple &&
      میانگیر->successful &&
      (نما == HB_OT_SHAPE_NORMALIZATION_نما_COMPOSED_DIACRITICS ||
       نما == HB_OT_SHAPE_NORMALIZATION_نما_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      میانگیر->پیام (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    میانگیر->clear_output ();
    شمار = میانگیر->len;
    بیعلامت صحيح starter = 0;
    (فراغ) میانگیر->بعدی_glyph ();
    طالما (میانگیر->idx < شمار /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      إذا (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&میانگیر->cur()))
      {
	إذا (/* If there's anything between the starter and this char, they should have CCC
	     * smaller than this character's. */
	    (starter == میانگیر->بیرون_len - 1 ||
	     info_cc (میانگیر->prev()) < info_cc (میانگیر->cur())) &&
	    /* And compose. */
	    c.compose (&c,
		       میانگیر->بیرون_info[starter].codepoint,
		       میانگیر->cur().codepoint,
		       &composed) &&
	    /* And the font has glyph for the composite. */
	    font->get_nominal_glyph (composed, &glyph))
	{
	  /* Composes. */
	  إذا (unlikely (!میانگیر->بعدی_glyph ())) اكسر; /* Copy to out-buffer. */
	  میانگیر->ادغام_بیرون_clusters (starter, میانگیر->بیرون_len);
	  میانگیر->بیرون_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  میانگیر->بیرون_info[starter].codepoint = composed;
	  میانگیر->بیرون_info[starter].normalizer_glyph_شاخص() = glyph;
	  hb_glyph_info_مجموعه_unicode_props (&میانگیر->بیرون_info[starter], میانگیر);

	  استمر;
	}
      }

      /* Blocked, or doesn't compose. */
      إذا (unlikely (!میانگیر->بعدی_glyph ())) اكسر;

      إذا (info_cc (میانگیر->prev()) == 0)
	starter = میانگیر->بیرون_len - 1;
    }
    میانگیر->متزامن ();
    (فراغ) میانگیر->پیام (font, "end compose");
  }
}


#پایان_اگر
