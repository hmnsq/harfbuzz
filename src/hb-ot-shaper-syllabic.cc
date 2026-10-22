/*
 * Copyright © 2021  Behdad Esfahbod.
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
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-syllabic.hh"


ਤਾਰਕਿਕ
hb_syllabic_ਪਾਓ_dotted_circles (hb_font_t *font,
				   hb_ਬਫਰ_t *ਬਫਰ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ broken_syllable_type,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dottedcircle_category,
				   ਪੂਰਨ repha_category,
				   ਪੂਰਨ dottedcircle_position)
{
  ਜੇ (unlikely (ਬਫਰ->flags & HB_ਬਫਰ_FLAG_DO_NOT_ਪਾਓ_DOTTED_CIRCLE))
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (likely (!(ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    ਜੇ (ਬਫਰ->messaging ())
      (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "skipped inserting dotted-circles because there is no broken syllables");
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (ਬਫਰ->messaging () &&
      !ਬਫਰ->ਸੁਨੇਹਾ (font, "start inserting dotted-circles"))
    ਵਾਪਸ ਝੂਠ;

  hb_codepoint_t dottedcircle_glyph;
  ਜੇ (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    ਵਾਪਸ ਝੂਠ;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  ਜੇ (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  ਬਫਰ->clear_output ();

  ਬਫਰ->idx = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_syllable = 0;
  ਜਦੋਂ (ਬਫਰ->idx < ਬਫਰ->len && ਬਫਰ->successful)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ syllable = ਬਫਰ->cur().syllable();
    ਜੇ (unlikely (ਆਖਰੀ_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      ਆਖਰੀ_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = ਬਫਰ->cur().cluster;
      ginfo.ਨਕਾਬ = ਬਫਰ->cur().ਨਕਾਬ;
      ginfo.syllable() = ਬਫਰ->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      ਜੇ (repha_category != -1)
      {
	ਜਦੋਂ (ਬਫਰ->idx < ਬਫਰ->len && ਬਫਰ->successful &&
	       ਆਖਰੀ_syllable == ਬਫਰ->cur().syllable() &&
	       ਬਫਰ->cur().ot_shaper_var_u8_category() == (ਬਿਨਾਨਿਸ਼ਾਨੀ) repha_category)
	  (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
      }

      (ਖਾਲੀ) ਬਫਰ->output_info (ginfo);
    }
    ਨਹੀਂਤਾਂ
      (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
  }
  ਬਫਰ->ਸਮਕਾਲੀ ();

  ਜੇ (ਬਫਰ->messaging ())
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end inserting dotted-circles");

  ਵਾਪਸ ਸੱਚ;
}

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
hb_syllabic_clear_var (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, syllable);
  ਵਾਪਸ ਝੂਠ;
}


#ਜੇ_ਅੰਤ
