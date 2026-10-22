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

#شامل "hb.hh"

#ifndef HB_NO_OT_SHAPE

#شامل "hb-ot-shaper-syllabic.hh"


منطقی
hb_syllabic_درج_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned صحیح broken_syllable_type,
				   unsigned صحیح dottedcircle_category,
				   صحیح repha_category,
				   صحیح dottedcircle_position)
{
  اگر (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_درج_DOTTED_CIRCLE))
    بازگشت نادرست;
  اگر (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    اگر (buffer->messaging ())
      (تهی) buffer->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    بازگشت نادرست;
  }

  اگر (buffer->messaging () &&
      !buffer->message (font, "start inserting dotted-circles"))
    بازگشت نادرست;

  hb_codepoint_t dottedcircle_glyph;
  اگر (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    بازگشت نادرست;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  اگر (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  buffer->clear_output ();

  buffer->idx = 0;
  unsigned صحیح last_syllable = 0;
  وقتی (buffer->idx < buffer->len && buffer->successful)
  {
    unsigned صحیح syllable = buffer->cur().syllable();
    اگر (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = buffer->cur().cluster;
      ginfo.mask = buffer->cur().mask;
      ginfo.syllable() = buffer->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      اگر (repha_category != -1)
      {
	وقتی (buffer->idx < buffer->len && buffer->successful &&
	       last_syllable == buffer->cur().syllable() &&
	       buffer->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (تهی) buffer->next_glyph ();
      }

      (تهی) buffer->output_info (ginfo);
    }
    وگرنه
      (تهی) buffer->next_glyph ();
  }
  buffer->همگام ();

  اگر (buffer->messaging ())
    (تهی) buffer->message (font, "end inserting dotted-circles");

  بازگشت درست;
}

HB_INTERNAL منطقی
hb_syllabic_clear_var (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, syllable);
  بازگشت نادرست;
}


#endif
