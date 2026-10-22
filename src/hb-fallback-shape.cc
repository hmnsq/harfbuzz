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

#ಅನ್ತರ್ಭಾವ "hb-shaper-impl.hh"

#ifndef HB_NO_FALLBACK_SHAPE

/*
 * shaper face data
 */

ಸಂರಚನಾ hb_fallback_face_data_t {};

hb_fallback_face_data_t *
_hb_fallback_shaper_face_data_create (hb_face_t *face HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ (hb_fallback_face_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

ಶೂನ್ಯ
_hb_fallback_shaper_face_data_destroy (hb_fallback_face_data_t *data HB_UNUSED)
{
}


/*
 * shaper font data
 */

ಸಂರಚನಾ hb_fallback_font_data_t {};

hb_fallback_font_data_t *
_hb_fallback_shaper_font_data_create (hb_font_t *font HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ (hb_fallback_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

ಶೂನ್ಯ
_hb_fallback_shaper_font_data_destroy (hb_fallback_font_data_t *data HB_UNUSED)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_fallback_shape (hb_shape_plan_t    *shape_plan HB_UNUSED,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    ಧ್ರುವ hb_feature_t *features HB_UNUSED,
		    unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features HB_UNUSED)
{
  hb_codepoint_t space;
  ತಾರ್ಕಿಕ has_space = (ತಾರ್ಕಿಕ) font->get_nominal_glyph (' ', &space);

  buffer->clear_positions ();

  hb_direction_t direction = buffer->props.direction;
  hb_unicode_funcs_t *unicode = buffer->unicode;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
  {
    ಯದಿ (has_space && unicode->is_default_ignorable (info[i].codepoint)) {
      info[i].codepoint = space;
      pos[i].x_advance = 0;
      pos[i].y_advance = 0;
      ಅನುವರ್ತ;
    }
    (ಶೂನ್ಯ) font->get_nominal_glyph (info[i].codepoint, &info[i].codepoint);
    font->get_glyph_advance_for_direction (info[i].codepoint,
					   direction,
					   &pos[i].x_advance,
					   &pos[i].y_advance);
    font->subtract_glyph_origin_for_direction (info[i].codepoint,
					       direction,
					       &pos[i].x_offset,
					       &pos[i].y_offset);
  }

  ಯದಿ (HB_DIRECTION_IS_BACKWARD (direction))
    hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

#endif
