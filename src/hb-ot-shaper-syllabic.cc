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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#અન્તર્ભાવ "hb-ot-shaper-syllabic.hh"


તાર્કિક
hb_syllabic_ઉમેરો_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned પૂર્ણાંક broken_syllable_type,
				   unsigned પૂર્ણાંક dottedcircle_category,
				   પૂર્ણાંક repha_category,
				   પૂર્ણાંક dottedcircle_position)
{
  જો (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_ઉમેરો_DOTTED_CIRCLE))
    પાછા ખોટું;
  જો (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    જો (buffer->messaging ())
      (શૂન્ય) buffer->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    પાછા ખોટું;
  }

  જો (buffer->messaging () &&
      !buffer->message (font, "start inserting dotted-circles"))
    પાછા ખોટું;

  hb_codepoint_t dottedcircle_glyph;
  જો (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    પાછા ખોટું;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  જો (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  buffer->clear_output ();

  buffer->idx = 0;
  unsigned પૂર્ણાંક last_syllable = 0;
  જ્યારે (buffer->idx < buffer->len && buffer->successful)
  {
    unsigned પૂર્ણાંક syllable = buffer->cur().syllable();
    જો (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = buffer->cur().cluster;
      ginfo.mask = buffer->cur().mask;
      ginfo.syllable() = buffer->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      જો (repha_category != -1)
      {
	જ્યારે (buffer->idx < buffer->len && buffer->successful &&
	       last_syllable == buffer->cur().syllable() &&
	       buffer->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (શૂન્ય) buffer->next_glyph ();
      }

      (શૂન્ય) buffer->output_info (ginfo);
    }
    નહીંતર
      (શૂન્ય) buffer->next_glyph ();
  }
  buffer->સમકાલિક ();

  જો (buffer->messaging ())
    (શૂન્ય) buffer->message (font, "end inserting dotted-circles");

  પાછા સાચું;
}

HB_INTERNAL તાર્કિક
hb_syllabic_clear_var (અચલ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, syllable);
  પાછા ખોટું;
}


#endif
