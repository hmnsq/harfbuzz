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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাব "hb-ot-shaper-syllabic.hh"


যৌক্তিক
hb_syllabic_ঢোকাও_dotted_circles (hb_font_t *font,
				   hb_বাফার_t *বাফার,
				   unsigned পূর্ণসংখ্যা broken_syllable_type,
				   unsigned পূর্ণসংখ্যা dottedcircle_category,
				   পূর্ণসংখ্যা repha_category,
				   পূর্ণসংখ্যা dottedcircle_position)
{
  যদি (unlikely (বাফার->flags & HB_বাফার_FLAG_DO_NOT_ঢোকাও_DOTTED_CIRCLE))
    ফেরত মিথ্যা;
  যদি (likely (!(বাফার->scratch_flags & HB_বাফার_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    যদি (বাফার->messaging ())
      (শূন্য) বাফার->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    ফেরত মিথ্যা;
  }

  যদি (বাফার->messaging () &&
      !বাফার->message (font, "start inserting dotted-circles"))
    ফেরত মিথ্যা;

  hb_codepoint_t dottedcircle_glyph;
  যদি (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    ফেরত মিথ্যা;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  যদি (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  বাফার->clear_output ();

  বাফার->idx = 0;
  unsigned পূর্ণসংখ্যা last_syllable = 0;
  যখন (বাফার->idx < বাফার->len && বাফার->successful)
  {
    unsigned পূর্ণসংখ্যা syllable = বাফার->cur().syllable();
    যদি (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = বাফার->cur().cluster;
      ginfo.mask = বাফার->cur().mask;
      ginfo.syllable() = বাফার->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      যদি (repha_category != -1)
      {
	যখন (বাফার->idx < বাফার->len && বাফার->successful &&
	       last_syllable == বাফার->cur().syllable() &&
	       বাফার->cur().ot_shaper_var_u8_category() == (unsigned) repha_category)
	  (শূন্য) বাফার->পরবর্তী_glyph ();
      }

      (শূন্য) বাফার->output_info (ginfo);
    }
    অন্যথা
      (শূন্য) বাফার->পরবর্তী_glyph ();
  }
  বাফার->সমলয় ();

  যদি (বাফার->messaging ())
    (শূন্য) বাফার->message (font, "end inserting dotted-circles");

  ফেরত সত্য;
}

HB_INTERNAL যৌক্তিক
hb_syllabic_clear_var (ধ্রুবক hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_বাফার_t *বাফার)
{
  HB_বাফার_DEALLOCATE_VAR (বাফার, syllable);
  ফেরত মিথ্যা;
}


#endif
