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

#ضمن "hb.hh"

#جے_بنا_تعریف HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-syllabic.hh"


منطقي
hb_syllabic_داخل_کرو_dotted_circles (hb_font_t *font,
				   hb_بفر_t *بفر,
				   بنانشانی صحيح broken_syllable_type,
				   بنانشانی صحيح dottedcircle_category,
				   صحيح repha_category,
				   صحيح dottedcircle_position)
{
  إذا (unlikely (بفر->flags & HB_بفر_FLAG_DO_NOT_داخل_کرو_DOTTED_CIRCLE))
    أرجع خطأ;
  إذا (likely (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    إذا (بفر->messaging ())
      (فراغ) بفر->سنیہا (font, "skipped inserting dotted-circles because there is no broken syllables");
    أرجع خطأ;
  }

  إذا (بفر->messaging () &&
      !بفر->سنیہا (font, "start inserting dotted-circles"))
    أرجع خطأ;

  hb_codepoint_t dottedcircle_glyph;
  إذا (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    أرجع خطأ;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  إذا (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  بفر->clear_output ();

  بفر->idx = 0;
  بنانشانی صحيح آخری_syllable = 0;
  طالما (بفر->idx < بفر->len && بفر->successful)
  {
    بنانشانی صحيح syllable = بفر->cur().syllable();
    إذا (unlikely (آخری_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      آخری_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = بفر->cur().cluster;
      ginfo.نقاب = بفر->cur().نقاب;
      ginfo.syllable() = بفر->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      إذا (repha_category != -1)
      {
	طالما (بفر->idx < بفر->len && بفر->successful &&
	       آخری_syllable == بفر->cur().syllable() &&
	       بفر->cur().ot_shaper_var_u8_category() == (بنانشانی) repha_category)
	  (فراغ) بفر->اگلا_glyph ();
      }

      (فراغ) بفر->output_info (ginfo);
    }
    وإلا
      (فراغ) بفر->اگلا_glyph ();
  }
  بفر->متزامن ();

  إذا (بفر->messaging ())
    (فراغ) بفر->سنیہا (font, "end inserting dotted-circles");

  أرجع سچ;
}

HB_اندرونی منطقي
hb_syllabic_clear_var (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t *بفر)
{
  HB_بفر_DEALLOCATE_VAR (بفر, syllable);
  أرجع خطأ;
}


#جے_انت
