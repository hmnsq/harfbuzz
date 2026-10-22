/*
 * Copyright © 2011  Google, Inc.
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
 *
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb-shaper-impl.hh"

#నిర్వచితం_కాకపోతే HB_NO_FALLBACK_SHAPE

/*
 * shaper face data
 */

నిర్మాణం hb_fallback_face_దత్తాంశం_t {};

hb_fallback_face_దత్తాంశం_t *
hb_fallback_shaper_face_దత్తాంశం_create (hb_face_t *face HB_వాడని)
{
  తిరిగి (hb_fallback_face_దత్తాంశం_t *) HB_SHAPER_దత్తాంశం_SUCCEEDED;
}

శూన్య
hb_fallback_shaper_face_దత్తాంశం_destroy (hb_fallback_face_దత్తాంశం_t *దత్తాంశం HB_వాడని)
{
}


/*
 * shaper font data
 */

నిర్మాణం hb_fallback_font_దత్తాంశం_t {};

hb_fallback_font_దత్తాంశం_t *
hb_fallback_shaper_font_దత్తాంశం_create (hb_font_t *font HB_వాడని)
{
  తిరిగి (hb_fallback_font_దత్తాంశం_t *) HB_SHAPER_దత్తాంశం_SUCCEEDED;
}

శూన్య
hb_fallback_shaper_font_దత్తాంశం_destroy (hb_fallback_font_దత్తాంశం_t *దత్తాంశం HB_వాడని)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_fallback_shape (hb_shape_plan_t    *shape_plan HB_వాడని,
		    hb_font_t          *font,
		    hb_బఫరు_t        *బఫరు,
		    స్థిరరాశి hb_feature_t *features HB_వాడని,
		    గుర్తులేని పూర్ణాంకం        అంకె_features HB_వాడని)
{
  hb_codepoint_t space;
  తార్కిక has_space = (తార్కిక) font->get_nominal_glyph (' ', &space);

  బఫరు->clear_positions ();

  hb_direction_t direction = బఫరు->props.direction;
  hb_unicode_funcs_t *unicode = బఫరు->unicode;
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  hb_glyph_position_t *pos = బఫరు->pos;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    ఒకవేళ (has_space && unicode->is_default_ignorable (info[i].codepoint)) {
      info[i].codepoint = space;
      pos[i].x_advance = 0;
      pos[i].y_advance = 0;
      కొనసాగు;
    }
    (శూన్య) font->get_nominal_glyph (info[i].codepoint, &info[i].codepoint);
    font->get_glyph_advance_for_direction (info[i].codepoint,
					   direction,
					   &pos[i].x_advance,
					   &pos[i].y_advance);
    font->వ్యవకలనం_glyph_origin_for_direction (info[i].codepoint,
					       direction,
					       &pos[i].x_స్థానభ్రంశం_2,
					       &pos[i].y_స్థానభ్రంశం_2);
  }

  ఒకవేళ (HB_DIRECTION_IS_BACKWARD (direction))
    hb_బఫరు_తిరగేయి (బఫరు);

  బఫరు->clear_glyph_flags ();

  తిరిగి నిజం;
}

#అయితే_ముగింపు
