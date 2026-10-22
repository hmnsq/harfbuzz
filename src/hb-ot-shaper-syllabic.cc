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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shaper-syllabic.hh"


తార్కిక
hb_syllabic_చొప్పించు_dotted_circles (hb_font_t *font,
				   hb_బఫరు_t *బఫరు,
				   గుర్తులేని పూర్ణాంకం broken_syllable_type,
				   గుర్తులేని పూర్ణాంకం dottedcircle_category,
				   పూర్ణాంకం repha_category,
				   పూర్ణాంకం dottedcircle_position)
{
  ఒకవేళ (unlikely (బఫరు->flags & HB_బఫరు_FLAG_DO_NOT_చొప్పించు_DOTTED_CIRCLE))
    తిరిగి అబద్ధం;
  ఒకవేళ (likely (!(బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    ఒకవేళ (బఫరు->messaging ())
      (శూన్య) బఫరు->సందేశం (font, "skipped inserting dotted-circles because there is no broken syllables");
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (బఫరు->messaging () &&
      !బఫరు->సందేశం (font, "start inserting dotted-circles"))
    తిరిగి అబద్ధం;

  hb_codepoint_t dottedcircle_glyph;
  ఒకవేళ (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    తిరిగి అబద్ధం;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  ఒకవేళ (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  బఫరు->clear_output ();

  బఫరు->idx = 0;
  గుర్తులేని పూర్ణాంకం చివరి_syllable = 0;
  అయితే (బఫరు->idx < బఫరు->len && బఫరు->successful)
  {
    గుర్తులేని పూర్ణాంకం syllable = బఫరు->cur().syllable();
    ఒకవేళ (unlikely (చివరి_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      చివరి_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = బఫరు->cur().cluster;
      ginfo.ముసుగు = బఫరు->cur().ముసుగు;
      ginfo.syllable() = బఫరు->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      ఒకవేళ (repha_category != -1)
      {
	అయితే (బఫరు->idx < బఫరు->len && బఫరు->successful &&
	       చివరి_syllable == బఫరు->cur().syllable() &&
	       బఫరు->cur().ot_shaper_var_u8_category() == (గుర్తులేని) repha_category)
	  (శూన్య) బఫరు->తదుపరి_glyph ();
      }

      (శూన్య) బఫరు->output_info (ginfo);
    }
    లేకపోతే
      (శూన్య) బఫరు->తదుపరి_glyph ();
  }
  బఫరు->సమకాలీకరణ ();

  ఒకవేళ (బఫరు->messaging ())
    (శూన్య) బఫరు->సందేశం (font, "end inserting dotted-circles");

  తిరిగి నిజం;
}

HB_అంతర్గత తార్కిక
hb_syllabic_clear_var (స్థిరరాశి hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_DEALLOCATE_VAR (బఫరు, syllable);
  తిరిగి అబద్ధం;
}


#అయితే_ముగింపు
