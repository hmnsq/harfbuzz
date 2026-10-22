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

#include "hb.hh"

#ifndef HB_NO_OT_SHAPE

#include "hb-ot-shape-normalize.hh"
#include "hb-ot-shaper.hh"
#include "hb-ot-shape.hh"


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


static anta_hstha void
set_glyph (hb_glyph_info_t &info, hb_font_t *font)
{
  (void) font->get_nominal_glyph (info.codepoint, &info.normalizer_glyph_index());
}

static anta_hstha void
output_char (hb_buffer_t *buffer, hb_codepoint_t unichar, hb_codepoint_t glyph)
{
  /* This is very confusing indeed. */
  buffer->cur().normalizer_glyph_index() = glyph;
  (void) buffer->output_glyph (unichar);
  _hb_glyph_info_set_unicode_props (&buffer->prev(), buffer);
}

static anta_hstha void
next_char (hb_buffer_t *buffer, hb_codepoint_t glyph)
{
  buffer->cur().normalizer_glyph_index() = glyph;
  (void) buffer->next_glyph ();
}

static anta_hstha void
skip_char (hb_buffer_t *buffer)
{
  buffer->skip_glyph ();
}

/* Returns 0 if didn't decompose, number of resulting characters otherwise. */
static anta_hstha unsigned ကိန်းပြည့်
decompose (const hb_ot_shape_normalize_context_t *c, bool shortest, hb_codepoint_t ab)
{
  hb_codepoint_t a = 0, b = 0, a_glyph = 0, b_glyph = 0;
  hb_buffer_t * const buffer = c->buffer;
  hb_font_t * const font = c->font;

  အကယ်၍ (!c->decompose (c, ab, &a, &b) ||
      (b && !font->get_nominal_glyph (b, &b_glyph)))
    ပြန်ပေး 0;

  bool has_a = (bool) font->get_nominal_glyph (a, &a_glyph);
  အကယ်၍ (shortest && has_a) {
    /* Output a and b */
    output_char (buffer, a, a_glyph);
    အကယ်၍ (likely (b)) {
      output_char (buffer, b, b_glyph);
      ပြန်ပေး 2;
    }
    ပြန်ပေး 1;
  }

  အကယ်၍ (unsigned ret = decompose (c, shortest, a)) {
    အကယ်၍ (b) {
      output_char (buffer, b, b_glyph);
      ပြန်ပေး ret + 1;
    }
    ပြန်ပေး ret;
  }

  အကယ်၍ (has_a) {
    output_char (buffer, a, a_glyph);
    အကယ်၍ (likely (b)) {
      output_char (buffer, b, b_glyph);
      ပြန်ပေး 2;
    }
    ပြန်ပေး 1;
  }

  ပြန်ပေး 0;
}

static anta_hstha void
decompose_current_character (const hb_ot_shape_normalize_context_t *c, bool shortest)
{
  hb_buffer_t * const buffer = c->buffer;
  hb_codepoint_t u = buffer->cur().codepoint;
  hb_codepoint_t glyph = 0;

  အကယ်၍ (shortest && c->font->get_nominal_glyph (u, &glyph, buffer->not_found))
  {
    next_char (buffer, glyph);
    ပြန်ပေး;
  }

  အကယ်၍ (decompose (c, shortest, u))
  {
    skip_char (buffer);
    ပြန်ပေး;
  }

  အကယ်၍ (!shortest && c->font->get_nominal_glyph (u, &glyph, buffer->not_found))
  {
    next_char (buffer, glyph);
    ပြန်ပေး;
  }

  အကယ်၍ (_hb_glyph_info_is_unicode_space (&buffer->cur()))
  {
    hb_codepoint_t space_glyph;
    hb_unicode_funcs_t::space_t space_type = buffer->unicode->space_fallback_type (u);
    အကယ်၍ (space_type != hb_unicode_funcs_t::NOT_SPACE &&
	(c->font->get_nominal_glyph (0x0020, &space_glyph) || (space_glyph = buffer->invisible)))
    {
      _hb_glyph_info_set_unicode_space_fallback_type (&buffer->cur(), space_type);
      next_char (buffer, space_glyph);
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_SPACE_FALLBACK;
      ပြန်ပေး;
    }
  }

  အကယ်၍ (u == 0x2011u)
  {
    /* U+2011 is the only sensible character that is a no-break version of another character
     * and not a space.  The space ones are handled already.  Handle this lone one. */
    hb_codepoint_t other_glyph;
    အကယ်၍ (c->font->get_nominal_glyph (0x2010u, &other_glyph))
    {
      next_char (buffer, other_glyph);
      ပြန်ပေး;
    }
  }

  next_char (buffer, glyph); /* glyph is initialized in earlier branches. */
}

static anta_hstha void
handle_variation_selector_cluster (const hb_ot_shape_normalize_context_t *c,
				   unsigned ကိန်းပြည့် end,
				   bool short_circuit HB_UNUSED)
{
  /* Currently if there's a variation-selector we give-up on normalization, it's just too hard. */
  hb_buffer_t * const buffer = c->buffer;
  hb_font_t * const font = c->font;
  အတွက် (; buffer->idx < end - 1 && buffer->successful;) {
    အကယ်၍ (unlikely (buffer->unicode->is_variation_selector (buffer->cur(+1).codepoint))) {
      အကယ်၍ (font->get_variation_glyph (buffer->cur().codepoint, buffer->cur(+1).codepoint, &buffer->cur().normalizer_glyph_index()))
      {
	hb_codepoint_t unicode = buffer->cur().codepoint;
	(void) buffer->replace_glyphs (2, 1, &unicode);
      }
      မဟုတ်ပါက
      {
	/* Just pass on the two characters separately, let GSUB do its magic. */
	set_glyph (buffer->cur(), font);
	(void) buffer->next_glyph ();

        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK;
	_hb_glyph_info_set_variation_selector (&buffer->cur(), true);
	အကယ်၍ (buffer->not_found_variation_selector != HB_CODEPOINT_INVALID)
	  _hb_glyph_info_clear_default_ignorable (&buffer->cur());

	set_glyph (buffer->cur(), font);
	(void) buffer->next_glyph ();
      }
      /* Skip any further variation selectors. */
      နေစဉ် (buffer->idx < end &&
	     buffer->successful &&
	     unlikely (buffer->unicode->is_variation_selector (buffer->cur().codepoint)))
      {
	set_glyph (buffer->cur(), font);
	(void) buffer->next_glyph ();
      }
    }
    မဟုတ်ပါက
    {
      set_glyph (buffer->cur(), font);
      (void) buffer->next_glyph ();
    }
  }
  အကယ်၍ (likely (buffer->idx < end))
  {
    set_glyph (buffer->cur(), font);
    (void) buffer->next_glyph ();
  }
}

static anta_hstha void
decompose_multi_char_cluster (const hb_ot_shape_normalize_context_t *c, unsigned ကိန်းပြည့် end, bool short_circuit)
{
  hb_buffer_t * const buffer = c->buffer;
  အတွက် (unsigned ကိန်းပြည့် i = buffer->idx; i < end && buffer->successful; i++)
    အကယ်၍ (unlikely (buffer->unicode->is_variation_selector (buffer->info[i].codepoint))) {
      handle_variation_selector_cluster (c, end, short_circuit);
      ပြန်ပေး;
    }

  နေစဉ် (buffer->idx < end && buffer->successful)
    decompose_current_character (c, short_circuit);
}


static ကိန်းပြည့်
compare_combining_class (const hb_glyph_info_t *pa, const hb_glyph_info_t *pb)
{
  unsigned ကိန်းပြည့် a = _hb_glyph_info_get_modified_combining_class (pa);
  unsigned ကိန်းပြည့် b = _hb_glyph_info_get_modified_combining_class (pb);

  ပြန်ပေး a < b ? -1 : a == b ? 0 : +1;
}


void
_hb_ot_shape_normalize (const hb_ot_shape_plan_t *plan,
			hb_buffer_t *buffer,
			hb_font_t *font)
{
  အကယ်၍ (unlikely (!buffer->len)) ပြန်ပေး;

  _hb_buffer_assert_unicode_vars (buffer);

  hb_ot_shape_normalization_mode_t mode = plan->shaper->normalization_preference;
  အကယ်၍ (mode == HB_OT_SHAPE_NORMALIZATION_MODE_AUTO)
  {
    အကယ်၍ (plan->has_gpos_mark)
      // https://github.com/harfbuzz/harfbuzz/issues/653#issuecomment-423905920
      //mode = HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED;
      mode = HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS;
    မဟုတ်ပါက
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

  bool always_short_circuit = mode == HB_OT_SHAPE_NORMALIZATION_MODE_NONE;
  bool might_short_circuit = always_short_circuit ||
			     (mode != HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED &&
			      mode != HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT);
  unsigned ကိန်းပြည့် count;

  /* We do a fairly straightforward yet custom normalization process in three
   * separate rounds: decompose, reorder, recompose (if desired).  Currently
   * this makes two buffer swaps.  We can make it faster by moving the last
   * two rounds into the inner loop for the first round, but it's more readable
   * this way. */


  /* First round, decompose */

  bool all_simple = true;
  အကယ်၍ (buffer->message (font, "start decompose"))
  {
    buffer->clear_output ();
    count = buffer->len;
    buffer->idx = 0;
    do
    {
      unsigned ကိန်းပြည့် end;
      အတွက် (end = buffer->idx + 1; end < count; end++)
	အကယ်၍ (_hb_glyph_info_is_unicode_mark (&buffer->info[end]))
	  bha_nga;

      အကယ်၍ (end < count)
	end--; /* Leave one base for the marks to cluster with. */

      /* From idx to end are simple clusters. */
      အကယ်၍ (might_short_circuit)
      {
	unsigned ကိန်းပြည့် done = font->get_nominal_glyphs (end - buffer->idx,
						      &buffer->cur().codepoint,
						      parimaa_na (buffer->info[0]),
						      &buffer->cur().normalizer_glyph_index(),
						      parimaa_na (buffer->info[0]));
	အကယ်၍ (unlikely (!buffer->next_glyphs (done))) bha_nga;
      }
      နေစဉ် (buffer->idx < end && buffer->successful)
	decompose_current_character (&c, might_short_circuit);

      အကယ်၍ (buffer->idx == count || !buffer->successful)
	bha_nga;

      all_simple = false;

      /* Find all the marks now. */
      အတွက် (end = buffer->idx + 1; end < count; end++)
	အကယ်၍ (!_hb_glyph_info_is_unicode_mark(&buffer->info[end]))
	  bha_nga;

      /* idx to end is one non-simple cluster. */
      decompose_multi_char_cluster (&c, end, always_short_circuit);
    }
    နေစဉ် (buffer->idx < count && buffer->successful);
    buffer->sync ();
    (void) buffer->message (font, "end decompose");
  }


  /* Second round, reorder (inplace) */

  အကယ်၍ (!all_simple && buffer->message(font, "start reorder"))
  {
    count = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
    {
      အကယ်၍ (_hb_glyph_info_get_modified_combining_class (&info[i]) == 0)
	continue;

      unsigned ကိန်းပြည့် end;
      အတွက် (end = i + 1; end < count; end++)
	အကယ်၍ (_hb_glyph_info_get_modified_combining_class (&info[end]) == 0)
	  bha_nga;

      /* We are going to do a O(n^2).  Only do this if the sequence is short. */
      အကယ်၍ (end - i > HB_OT_SHAPE_MAX_COMBINING_MARKS) {
	i = end;
	continue;
      }

      buffer->sort (i, end, compare_combining_class);

      အကယ်၍ (plan->shaper->reorder_marks)
	plan->shaper->reorder_marks (plan, buffer, i, end);

      i = end;
    }
    (void) buffer->message(font, "end reorder");
  }
  အကယ်၍ (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_CGJ)
  {
    /* For all CGJ, check if it prevented any reordering at all.
     * If it did NOT, then make it skippable.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     */
    unsigned count = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    အတွက် (unsigned ကိန်းပြည့် i = 1; i + 1 < count; i++)
      အကယ်၍ (info[i].codepoint == 0x034Fu/*CGJ*/ &&
	  (info_cc(info[i+1]) == 0 || info_cc(info[i-1]) <= info_cc(info[i+1])))
      {
	_hb_glyph_info_unhide (&info[i]);
      }
  }


  /* Third round, recompose */

  အကယ်၍ (!all_simple &&
      buffer->successful &&
      (mode == HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS ||
       mode == HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT) &&
      buffer->message (font, "start compose"))
  {
    /* As noted in the comment earlier, we don't try to combine
     * ccc=0 chars with their previous Starter. */

    buffer->clear_output ();
    count = buffer->len;
    unsigned ကိန်းပြည့် starter = 0;
    (void) buffer->next_glyph ();
    နေစဉ် (buffer->idx < count /* No need for: && buffer->successful */)
    {
      hb_codepoint_t composed, glyph;
      အကယ်၍ (/* We don't try to compose a non-mark character with it's preceding starter.
	   * This is both an optimization to avoid trying to compose every two neighboring
	   * glyphs in most scripts AND a desired feature for Hangul.  Apparently Hangul
	   * fonts are not designed to mix-and-match pre-composed syllables and Jamo. */
	  _hb_glyph_info_is_unicode_mark(&buffer->cur()))
      {
	အကယ်၍ (/* If there's anything between the starter and this char, they should have CCC
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
	  အကယ်၍ (unlikely (!buffer->next_glyph ())) bha_nga; /* Copy to out-buffer. */
	  buffer->merge_out_clusters (starter, buffer->out_len);
	  buffer->out_len--; /* Remove the second composable. */
	  /* Modify starter and carry on. */
	  buffer->out_info[starter].codepoint = composed;
	  buffer->out_info[starter].normalizer_glyph_index() = glyph;
	  _hb_glyph_info_set_unicode_props (&buffer->out_info[starter], buffer);

	  continue;
	}
      }

      /* Blocked, or doesn't compose. */
      အကယ်၍ (unlikely (!buffer->next_glyph ())) bha_nga;

      အကယ်၍ (info_cc (buffer->prev()) == 0)
	starter = buffer->out_len - 1;
    }
    buffer->sync ();
    (void) buffer->message (font, "end compose");
  }
}


#endif
