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

#ضمن "hb-shaper-impl.hh"

#که_نه_تعریف_شوی HB_NO_FALLBACK_SHAPE

/*
 * shaper face data
 */

بنية hb_fallback_face_معلومات_t {};

hb_fallback_face_معلومات_t *
hb_fallback_shaper_face_معلومات_create (hb_face_t *face HB_ناکارول_شوی)
{
  أرجع (hb_fallback_face_معلومات_t *) HB_SHAPER_معلومات_SUCCEEDED;
}

فراغ
hb_fallback_shaper_face_معلومات_destroy (hb_fallback_face_معلومات_t *معلومات HB_ناکارول_شوی)
{
}


/*
 * shaper font data
 */

بنية hb_fallback_font_معلومات_t {};

hb_fallback_font_معلومات_t *
hb_fallback_shaper_font_معلومات_create (hb_font_t *font HB_ناکارول_شوی)
{
  أرجع (hb_fallback_font_معلومات_t *) HB_SHAPER_معلومات_SUCCEEDED;
}

فراغ
hb_fallback_shaper_font_معلومات_destroy (hb_fallback_font_معلومات_t *معلومات HB_ناکارول_شوی)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_fallback_shape (hb_shape_plan_t    *shape_plan HB_ناکارول_شوی,
		    hb_font_t          *font,
		    hb_بفر_t        *بفر,
		    ثابت hb_feature_t *features HB_ناکارول_شوی,
		    بینښه صحيح        عدد_features HB_ناکارول_شوی)
{
  hb_codepoint_t space;
  منطقي has_space = (منطقي) font->get_nominal_glyph (' ', &space);

  بفر->clear_positions ();

  hb_direction_t direction = بفر->props.direction;
  hb_unicode_funcs_t *unicode = بفر->unicode;
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    إذا (has_space && unicode->is_default_ignorable (info[i].codepoint)) {
      info[i].codepoint = space;
      pos[i].x_advance = 0;
      pos[i].y_advance = 0;
      استمر;
    }
    (فراغ) font->get_nominal_glyph (info[i].codepoint, &info[i].codepoint);
    font->get_glyph_advance_for_direction (info[i].codepoint,
					   direction,
					   &pos[i].x_advance,
					   &pos[i].y_advance);
    font->کمول_glyph_origin_for_direction (info[i].codepoint,
					       direction,
					       &pos[i].x_دننه_واټن_2,
					       &pos[i].y_دننه_واټن_2);
  }

  إذا (HB_DIRECTION_IS_BACKWARD (direction))
    hb_بفر_سرچپه (بفر);

  بفر->clear_glyph_flags ();

  أرجع ریښتیا;
}

#د_که_پای
