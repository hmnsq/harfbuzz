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

#অন্তর্ভাৱ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাৱ "hb-ot-shaper-syllabic.hh"


তার্কিক
hb_syllabic_ঢোকাও_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned পূর্ণাঙ্ক broken_syllable_type,
				   unsigned পূর্ণাঙ্ক dottedcircle_category,
				   পূর্ণাঙ্ক repha_category,
				   পূর্ণাঙ্ক dottedcircle_position)
{
  যদি (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_ঢোকাও_DOTTED_CIRCLE))
    ঘূৰাই মিছা;
  যদি (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    যদি (buffer->messaging ())
      (শূন্য) buffer->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    ঘূৰাই মিছা;
  }

  যদি (buffer->messaging () &&
      !buffer->message (font, "start inserting dotted-circles"))
    ঘূৰাই মিছা;

  hb_codepoint_t dottedcircle_glyph;
  যদি (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    ঘূৰাই মিছা;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  যদি (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  buffer->clear_output ();

  buffer->idx = 0;
  unsigned পূর্ণাঙ্ক last_syllable = 0;
  যেতিয়ালৈ (buffer->idx < buffer->len && buffer->successful)
  {
    unsigned পূর্ণাঙ্ক syllable = buffer->cur().syllable();
    যদি (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = buffer->cur().cluster;
      ginfo.mask = buffer->cur().mask;
      ginfo.syllable() = buffer->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      যদি (repha_category != -1)
      {
	যেতিয়ালৈ (buffer->idx < buffer->len && buffer->successful &&
	       last_syllable == buffer->cur().syllable() &&
	       buffer->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (শূন্য) buffer->next_glyph ();
      }

      (শূন্য) buffer->output_info (ginfo);
    }
    অন্যথা
      (শূন্য) buffer->next_glyph ();
  }
  buffer->সমকাল ();

  যদি (buffer->messaging ())
    (শূন্য) buffer->message (font, "end inserting dotted-circles");

  ঘূৰাই সঁচা;
}

HB_INTERNAL তার্কিক
hb_syllabic_clear_var (ধ্রুৱ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, syllable);
  ঘূৰাই মিছা;
}


#endif
