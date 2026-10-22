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

#include "hb.hh"

#ifndef HB_NO_OT_SHAPE

#include "hb-ot-shaper-syllabic.hh"


bool
hb_syllabic_insert_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned จำนวนเต็ม broken_syllable_type,
				   unsigned จำนวนเต็ม dottedcircle_category,
				   จำนวนเต็ม repha_category,
				   จำนวนเต็ม dottedcircle_position)
{
  ถ้า (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE))
    คืน false;
  ถ้า (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    ถ้า (buffer->messaging ())
      (void) buffer->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    คืน false;
  }

  ถ้า (buffer->messaging () &&
      !buffer->message (font, "start inserting dotted-circles"))
    คืน false;

  hb_codepoint_t dottedcircle_glyph;
  ถ้า (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    คืน false;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  ถ้า (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  buffer->clear_output ();

  buffer->idx = 0;
  unsigned จำนวนเต็ม last_syllable = 0;
  ขณะ (buffer->idx < buffer->len && buffer->successful)
  {
    unsigned จำนวนเต็ม syllable = buffer->cur().syllable();
    ถ้า (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = buffer->cur().cluster;
      ginfo.mask = buffer->cur().mask;
      ginfo.syllable() = buffer->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      ถ้า (repha_category != -1)
      {
	ขณะ (buffer->idx < buffer->len && buffer->successful &&
	       last_syllable == buffer->cur().syllable() &&
	       buffer->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (void) buffer->next_glyph ();
      }

      (void) buffer->output_info (ginfo);
    }
    มิฉะนั้น
      (void) buffer->next_glyph ();
  }
  buffer->sync ();

  ถ้า (buffer->messaging ())
    (void) buffer->message (font, "end inserting dotted-circles");

  คืน true;
}

HB_INTERNAL bool
hb_syllabic_clear_var (const hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, syllable);
  คืน false;
}


#endif
