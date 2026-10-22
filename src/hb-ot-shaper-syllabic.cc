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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-syllabic.hh"


ತಾರ್ಕಿಕ
hb_syllabic_ಸೇರಿಸು_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned ಪೂರ್ಣಾಙ್ಕ broken_syllable_type,
				   unsigned ಪೂರ್ಣಾಙ್ಕ dottedcircle_category,
				   ಪೂರ್ಣಾಙ್ಕ repha_category,
				   ಪೂರ್ಣಾಙ್ಕ dottedcircle_position)
{
  ಯದಿ (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_ಸೇರಿಸು_DOTTED_CIRCLE))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    ಯದಿ (buffer->messaging ())
      (ಶೂನ್ಯ) buffer->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಯದಿ (buffer->messaging () &&
      !buffer->message (font, "start inserting dotted-circles"))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_codepoint_t dottedcircle_glyph;
  ಯದಿ (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  ಯದಿ (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  buffer->clear_output ();

  buffer->idx = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ last_syllable = 0;
  ಯಾವತ್ (buffer->idx < buffer->len && buffer->successful)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ syllable = buffer->cur().syllable();
    ಯದಿ (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = buffer->cur().cluster;
      ginfo.mask = buffer->cur().mask;
      ginfo.syllable() = buffer->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      ಯದಿ (repha_category != -1)
      {
	ಯಾವತ್ (buffer->idx < buffer->len && buffer->successful &&
	       last_syllable == buffer->cur().syllable() &&
	       buffer->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (ಶೂನ್ಯ) buffer->next_glyph ();
      }

      (ಶೂನ್ಯ) buffer->output_info (ginfo);
    }
    ಅನ್ಯಥಾ
      (ಶೂನ್ಯ) buffer->next_glyph ();
  }
  buffer->ಸಮಕಾಲ ();

  ಯದಿ (buffer->messaging ())
    (ಶೂನ್ಯ) buffer->message (font, "end inserting dotted-circles");

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

HB_INTERNAL ತಾರ್ಕಿಕ
hb_syllabic_clear_var (ಧ್ರುವ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, syllable);
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}


#endif
