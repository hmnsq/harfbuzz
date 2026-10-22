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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#അന്തര്ഭാവ "hb-ot-shaper-syllabic.hh"


തർക്കം
hb_syllabic_തിരുകുക_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned പൂർണ്ണസംഖ്യ broken_syllable_type,
				   unsigned പൂർണ്ണസംഖ്യ dottedcircle_category,
				   പൂർണ്ണസംഖ്യ repha_category,
				   പൂർണ്ണസംഖ്യ dottedcircle_position)
{
  എങ്കിൽ (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_തിരുകുക_DOTTED_CIRCLE))
    തിരികെ അസത്യം;
  എങ്കിൽ (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    എങ്കിൽ (buffer->messaging ())
      (ശൂന്യം) buffer->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    തിരികെ അസത്യം;
  }

  എങ്കിൽ (buffer->messaging () &&
      !buffer->message (font, "start inserting dotted-circles"))
    തിരികെ അസത്യം;

  hb_codepoint_t dottedcircle_glyph;
  എങ്കിൽ (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    തിരികെ അസത്യം;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  എങ്കിൽ (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  buffer->clear_output ();

  buffer->idx = 0;
  unsigned പൂർണ്ണസംഖ്യ last_syllable = 0;
  ആകുമ്പോൾ (buffer->idx < buffer->len && buffer->successful)
  {
    unsigned പൂർണ്ണസംഖ്യ syllable = buffer->cur().syllable();
    എങ്കിൽ (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = buffer->cur().cluster;
      ginfo.mask = buffer->cur().mask;
      ginfo.syllable() = buffer->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      എങ്കിൽ (repha_category != -1)
      {
	ആകുമ്പോൾ (buffer->idx < buffer->len && buffer->successful &&
	       last_syllable == buffer->cur().syllable() &&
	       buffer->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (ശൂന്യം) buffer->next_glyph ();
      }

      (ശൂന്യം) buffer->output_info (ginfo);
    }
    അല്ലെങ്കിൽ
      (ശൂന്യം) buffer->next_glyph ();
  }
  buffer->സമന്വയം ();

  എങ്കിൽ (buffer->messaging ())
    (ശൂന്യം) buffer->message (font, "end inserting dotted-circles");

  തിരികെ സത്യം;
}

HB_INTERNAL തർക്കം
hb_syllabic_clear_var (സ്ഥിരം hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, syllable);
  തിരികെ അസത്യം;
}


#endif
