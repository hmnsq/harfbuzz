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

#اگر_تعریف_نشده HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-syllabic.hh"


منطقي
hb_syllabic_درج_dotted_circles (hb_font_t *font,
				   hb_میانگیر_t *میانگیر,
				   بیعلامت صحيح broken_syllable_type,
				   بیعلامت صحيح dottedcircle_category,
				   صحيح repha_category,
				   صحيح dottedcircle_position)
{
  إذا (unlikely (میانگیر->flags & HB_میانگیر_FLAG_DO_NOT_درج_DOTTED_CIRCLE))
    أرجع خطأ;
  إذا (likely (!(میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    إذا (میانگیر->messaging ())
      (فراغ) میانگیر->پیام (font, "skipped inserting dotted-circles because there is no broken syllables");
    أرجع خطأ;
  }

  إذا (میانگیر->messaging () &&
      !میانگیر->پیام (font, "start inserting dotted-circles"))
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

  میانگیر->clear_output ();

  میانگیر->idx = 0;
  بیعلامت صحيح آخرین_syllable = 0;
  طالما (میانگیر->idx < میانگیر->len && میانگیر->successful)
  {
    بیعلامت صحيح syllable = میانگیر->cur().syllable();
    إذا (unlikely (آخرین_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      آخرین_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = میانگیر->cur().cluster;
      ginfo.ماسک = میانگیر->cur().ماسک;
      ginfo.syllable() = میانگیر->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      إذا (repha_category != -1)
      {
	طالما (میانگیر->idx < میانگیر->len && میانگیر->successful &&
	       آخرین_syllable == میانگیر->cur().syllable() &&
	       میانگیر->cur().ot_shaper_var_u8_category() == (بیعلامت) repha_category)
	  (فراغ) میانگیر->بعدی_glyph ();
      }

      (فراغ) میانگیر->output_info (ginfo);
    }
    وإلا
      (فراغ) میانگیر->بعدی_glyph ();
  }
  میانگیر->متزامن ();

  إذا (میانگیر->messaging ())
    (فراغ) میانگیر->پیام (font, "end inserting dotted-circles");

  أرجع درست;
}

HB_داخلی منطقي
hb_syllabic_clear_var (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_میانگیر_t *میانگیر)
{
  HB_میانگیر_DEALLOCATE_VAR (میانگیر, syllable);
  أرجع خطأ;
}


#پایان_اگر
