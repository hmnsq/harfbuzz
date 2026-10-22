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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#समावेश "hb-ot-shaper-syllabic.hh"


bool
hb_syllabic_जोड़ो_dotted_circles (hb_font_t *font,
				   hb_बफर_t *बफर,
				   अचिन्हित पूर्णांक broken_syllable_type,
				   अचिन्हित पूर्णांक dottedcircle_category,
				   पूर्णांक repha_category,
				   पूर्णांक dottedcircle_position)
{
  अगर (unlikely (बफर->flags & HB_बफर_FLAG_DO_NOT_जोड़ो_DOTTED_CIRCLE))
    वापस false;
  अगर (likely (!(बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE)))
  {
    अगर (बफर->messaging ())
      (व्योम) बफर->message (font, "skipped inserting dotted-circles because there is no broken syllables");
    वापस false;
  }

  अगर (बफर->messaging () &&
      !बफर->message (font, "start inserting dotted-circles"))
    वापस false;

  hb_codepoint_t dottedcircle_glyph;
  अगर (!font->get_nominal_glyph (0x25CCu, &dottedcircle_glyph))
    वापस false;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  dottedcircle.ot_shaper_var_u8_category() = dottedcircle_category;
  अगर (dottedcircle_position != -1)
    dottedcircle.ot_shaper_var_u8_auxiliary() = dottedcircle_position;
  dottedcircle.codepoint = dottedcircle_glyph;

  बफर->clear_output ();

  बफर->idx = 0;
  अचिन्हित पूर्णांक last_syllable = 0;
  जबतक (बफर->idx < बफर->len && बफर->successful)
  {
    अचिन्हित पूर्णांक syllable = बफर->cur().syllable();
    अगर (unlikely (last_syllable != syllable && (syllable & 0x0F) == broken_syllable_type))
    {
      last_syllable = syllable;

      hb_glyph_info_t ginfo = dottedcircle;
      ginfo.cluster = बफर->cur().cluster;
      ginfo.mask = बफर->cur().mask;
      ginfo.syllable() = बफर->cur().syllable();

      /* Insert dottedcircle after possible Repha. */
      अगर (repha_category != -1)
      {
	जबतक (बफर->idx < बफर->len && बफर->successful &&
	       last_syllable == बफर->cur().syllable() &&
	       बफर->cur().ot_shaper_var_u8_category() == (अचिन्हित) repha_category)
	  (व्योम) बफर->अग्र_glyph ();
      }

      (व्योम) बफर->output_info (ginfo);
    }
    अन्यथा
      (व्योम) बफर->अग्र_glyph ();
  }
  बफर->sync ();

  अगर (बफर->messaging ())
    (व्योम) बफर->message (font, "end inserting dotted-circles");

  वापस true;
}

HB_INTERNAL bool
hb_syllabic_clear_var (स्थिर hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_बफर_t *बफर)
{
  HB_बफर_DEALLOCATE_VAR (बफर, syllable);
  वापस false;
}


#पूर्ण_अगर
