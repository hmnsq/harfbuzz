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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-syllabic.hh"


ତାର୍କିକ
hb_syllabic_ଭର୍ତ୍ତି_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା broken_syllable_type,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dottedcircle_category,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା repha_category,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା dottedcircle_position)
{
  ଯଦି (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_ଭର୍ତ୍ତି_DOTTED_CIRCLE))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    ଯଦି (buffer->messaging ())
      (ଶୂନ୍ୟ) buffer->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଯଦି (buffer->messaging () &&
      !buffer->message (font, "start inserting dotted-circles"))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_codepoint_t dottedcircle_glyph;
  ଯଦି (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  ଯଦି (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  buffer->clear_output ();

  buffer->idx = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_syllable = 0;
  ଯେତେବେଳେ (buffer->idx < buffer->len && buffer->successful)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା syllable = buffer->cur().syllable();
    ଯଦି (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = buffer->cur().cluster;
      ginfo.mask = buffer->cur().mask;
      ginfo.syllable() = buffer->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      ଯଦି (repha_category != -1)
      {
	ଯେତେବେଳେ (buffer->idx < buffer->len && buffer->successful &&
	       last_syllable == buffer->cur().syllable() &&
	       buffer->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (ଶୂନ୍ୟ) buffer->next_glyph ();
      }

      (ଶୂନ୍ୟ) buffer->output_info (ginfo);
    }
    ଅନ୍ୟଥା
      (ଶୂନ୍ୟ) buffer->next_glyph ();
  }
  buffer->ସମକାଳ ();

  ଯଦି (buffer->messaging ())
    (ଶୂନ୍ୟ) buffer->message (font, "end inserting dotted-circles");

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

HB_INTERNAL ତାର୍କିକ
hb_syllabic_clear_var (ଅଚଳ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, syllable);
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}


#endif
