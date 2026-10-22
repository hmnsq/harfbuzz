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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#அந்தர்பாவ "hb-ot-shaper-syllabic.hh"


தருக்க
hb_syllabic_செருகு_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned முழு broken_syllable_type,
				   unsigned முழு dottedcircle_category,
				   முழு repha_category,
				   முழு dottedcircle_position)
{
  எனில் (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_செருகு_DOTTED_CIRCLE))
    திரும்பு பொய்;
  எனில் (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    எனில் (buffer->messaging ())
      (வெறுமை) buffer->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    திரும்பு பொய்;
  }

  எனில் (buffer->messaging () &&
      !buffer->message (font, "start inserting dotted-circles"))
    திரும்பு பொய்;

  hb_codepoint_t dottedcircle_glyph;
  எனில் (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    திரும்பு பொய்;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  எனில் (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  buffer->clear_output ();

  buffer->idx = 0;
  unsigned முழு last_syllable = 0;
  போது (buffer->idx < buffer->len && buffer->successful)
  {
    unsigned முழு syllable = buffer->cur().syllable();
    எனில் (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = buffer->cur().cluster;
      ginfo.mask = buffer->cur().mask;
      ginfo.syllable() = buffer->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      எனில் (repha_category != -1)
      {
	போது (buffer->idx < buffer->len && buffer->successful &&
	       last_syllable == buffer->cur().syllable() &&
	       buffer->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (வெறுமை) buffer->next_glyph ();
      }

      (வெறுமை) buffer->output_info (ginfo);
    }
    இல்லையெனில்
      (வெறுமை) buffer->next_glyph ();
  }
  buffer->ஒத்திசை ();

  எனில் (buffer->messaging ())
    (வெறுமை) buffer->message (font, "end inserting dotted-circles");

  திரும்பு உண்மை;
}

HB_INTERNAL தருக்க
hb_syllabic_clear_var (மாறிலி hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, syllable);
  திரும்பு பொய்;
}


#endif
