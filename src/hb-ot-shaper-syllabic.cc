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

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#شامل "hb-ot-shaper-syllabic.hh"


منطقی
hb_syllabic_داخل_کرو_dotted_circles (hb_font_t *font,
				   hb_بفر_t *بفر,
				   غیرعلامتی صحیح broken_syllable_type,
				   غیرعلامتی صحیح dottedcircle_category,
				   صحیح repha_category,
				   صحیح dottedcircle_position)
{
  اگر (unlikely (بفر->flags & HB_بفر_FLAG_DO_NOT_داخل_کرو_DOTTED_CIRCLE))
    واپس جھوٹ;
  اگر (likely (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    اگر (بفر->messaging ())
      (خالی) بفر->پیغام (font, "skipped inserting dotted-circles because there is no broken syllables");
    واپس جھوٹ;
  }

  اگر (بفر->messaging () &&
      !بفر->پیغام (font, "start inserting dotted-circles"))
    واپس جھوٹ;

  hb_codepoint_t dottedcircle_glyph;
  اگر (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    واپس جھوٹ;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  اگر (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  بفر->clear_output ();

  بفر->idx = 0;
  غیرعلامتی صحیح آخری_syllable = 0;
  جب (بفر->idx < بفر->len && بفر->successful)
  {
    غیرعلامتی صحیح syllable = بفر->cur().syllable();
    اگر (unlikely (آخری_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      آخری_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = بفر->cur().cluster;
      ginfo.نقاب = بفر->cur().نقاب;
      ginfo.syllable() = بفر->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      اگر (repha_category != -1)
      {
	جب (بفر->idx < بفر->len && بفر->successful &&
	       آخری_syllable == بفر->cur().syllable() &&
	       بفر->cur().ot_shaper_var_u8_category() == (غیرعلامتی) repha_category)
	  (خالی) بفر->اگلا_glyph ();
      }

      (خالی) بفر->output_info (ginfo);
    }
    ورنہ
      (خالی) بفر->اگلا_glyph ();
  }
  بفر->ہمآہنگ ();

  اگر (بفر->messaging ())
    (خالی) بفر->پیغام (font, "end inserting dotted-circles");

  واپس سچ;
}

HB_اندرونی منطقی
hb_syllabic_clear_var (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t *بفر)
{
  HB_بفر_DEALLOCATE_VAR (بفر, syllable);
  واپس جھوٹ;
}


#اگر_ختم
