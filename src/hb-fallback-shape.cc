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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_FALLBACK_SHAPE

/*
 * shaper face data
 */

ਬਣਤਰ hb_fallback_face_ਅੰਕੜੇ_t {};

hb_fallback_face_ਅੰਕੜੇ_t *
hb_fallback_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ (hb_fallback_face_ਅੰਕੜੇ_t *) HB_SHAPER_ਅੰਕੜੇ_SUCCEEDED;
}

ਖਾਲੀ
hb_fallback_shaper_face_ਅੰਕੜੇ_destroy (hb_fallback_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
}


/*
 * shaper font data
 */

ਬਣਤਰ hb_fallback_font_ਅੰਕੜੇ_t {};

hb_fallback_font_ਅੰਕੜੇ_t *
hb_fallback_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ (hb_fallback_font_ਅੰਕੜੇ_t *) HB_SHAPER_ਅੰਕੜੇ_SUCCEEDED;
}

ਖਾਲੀ
hb_fallback_shaper_font_ਅੰਕੜੇ_destroy (hb_fallback_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_fallback_shape (hb_shape_plan_t    *shape_plan HB_ਅਣਵਰਤਿਆ,
		    hb_font_t          *font,
		    hb_ਬਫਰ_t        *ਬਫਰ,
		    ਅਚਲ hb_feature_t *features HB_ਅਣਵਰਤਿਆ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features HB_ਅਣਵਰਤਿਆ)
{
  hb_codepoint_t space;
  ਤਾਰਕਿਕ has_space = (ਤਾਰਕਿਕ) font->get_nominal_glyph (' ', &space);

  ਬਫਰ->clear_positions ();

  hb_direction_t direction = ਬਫਰ->props.direction;
  hb_unicode_funcs_t *unicode = ਬਫਰ->unicode;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  hb_glyph_position_t *pos = ਬਫਰ->pos;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (has_space && unicode->is_default_ignorable (info[i].codepoint)) {
      info[i].codepoint = space;
      pos[i].x_advance = 0;
      pos[i].y_advance = 0;
      ਜਾਰੀ;
    }
    (ਖਾਲੀ) font->get_nominal_glyph (info[i].codepoint, &info[i].codepoint);
    font->get_glyph_advance_for_direction (info[i].codepoint,
					   direction,
					   &pos[i].x_advance,
					   &pos[i].y_advance);
    font->ਘਟਾਓ_glyph_origin_for_direction (info[i].codepoint,
					       direction,
					       &pos[i].x_ਵਿੱਥ_2,
					       &pos[i].y_ਵਿੱਥ_2);
  }

  ਜੇ (HB_DIRECTION_IS_BACKWARD (direction))
    hb_ਬਫਰ_ਉਲਟ (ਬਫਰ);

  ਬਫਰ->clear_glyph_flags ();

  ਵਾਪਸ ਸੱਚ;
}

#ਜੇ_ਅੰਤ
