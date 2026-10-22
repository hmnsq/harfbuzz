/*
 * Copyright © 2019  Ebrahim Byagowi
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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_STYLE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-avar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-fvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-stat-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-head-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-post-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face.hh"

/**
 * SECTION:hb-style
 * @title: hb-style
 * @short_description: Font Styles
 * @include: hb.h
 *
 * Functions for fetching style information from fonts.
 **/

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
hb_angle_ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ (ਦਸ਼ਮਲਵ a)
{
  ਵਾਪਸ tanf (a * -HB_PI / 180.f);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
hb_ਅਨੁਪਾਤ_ਮੰਜ਼ਿਲ_angle (ਦਸ਼ਮਲਵ r)
{
  ਵਾਪਸ atanf (r) * -180.f / HB_PI;
}

/**
 * hb_style_get_value:
 * @font: a #hb_font_t object.
 * @style_tag: a style tag.
 *
 * Searches variation axes of a #hb_font_t object for a specific axis first,
 * if not set, first tries to get default style values in `STAT` table
 * then tries to polyfill from different tables of the font.
 *
 * Returns: Corresponding axis or default value to a style tag.
 *
 * Since: 3.0.0
 **/
ਦਸ਼ਮਲਵ
hb_style_get_ਮੁੱਲ (hb_font_t *font, hb_style_ਟੈਗ_t style_ਟੈਗ)
{
  ਜੇ (unlikely (style_ਟੈਗ == HB_STYLE_ਟੈਗ_SLANT_ਅਨੁਪਾਤ))
    ਵਾਪਸ hb_angle_ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ (hb_style_get_ਮੁੱਲ (font, HB_STYLE_ਟੈਗ_SLANT_ANGLE));

  hb_face_t *face = font->face;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  hb_ot_var_axis_info_t axis;
  ਜੇ (hb_ot_var_ਲੱਭੋ_axis_info (face, style_ਟੈਗ, &axis))
  {
    ਜੇ (axis.axis_ਸੂਚਕਾਂਕ < font->ਅੰਕ_coords) ਵਾਪਸ font->design_coords[axis.axis_ਸੂਚਕਾਂਕ];
    /* If a face is variable, fvar's default_value is better than STAT records */
    ਵਾਪਸ axis.default_ਮੁੱਲ;
  }
#ਜੇ_ਅੰਤ

  ਜੇ (style_ਟੈਗ == HB_STYLE_ਟੈਗ_OPTICAL_ਮਾਪ && font->ptem)
    ਵਾਪਸ font->ptem;

  /* STAT */
  ਦਸ਼ਮਲਵ value;
  ਜੇ (face->ਸਾਰਣੀ.STAT->get_ਮੁੱਲ (style_ਟੈਗ, &value))
    ਵਾਪਸ value;

  ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) style_ਟੈਗ)
  {
  ਹਾਲਤ HB_STYLE_ਟੈਗ_ITALIC:
    ਵਾਪਸ face->ਸਾਰਣੀ.OS2->is_italic () || face->ਸਾਰਣੀ.ਸਿਰ_2->is_italic () ? 1 : 0;
  ਹਾਲਤ HB_STYLE_ਟੈਗ_OPTICAL_ਮਾਪ:
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਹੇਠਲਾ, design, ਉੱਤਲਾ;
    ਵਾਪਸ face->ਸਾਰਣੀ.OS2->v5 ().get_optical_ਮਾਪ (&ਹੇਠਲਾ, &ਉੱਤਲਾ)
	   ? (ਦਸ਼ਮਲਵ) (ਹੇਠਲਾ + ਉੱਤਲਾ) / 2.f
	   : hb_ot_layout_get_ਮਾਪ_params (face, &design, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ)
	   ? design / 10.f
	   : 12.f;
  }
  ਹਾਲਤ HB_STYLE_ਟੈਗ_SLANT_ANGLE:
  {
    ਦਸ਼ਮਲਵ angle = face->ਸਾਰਣੀ.post->ਸਾਰਣੀ->italicAngle.ਮੰਜ਼ਿਲ_float ();

    ਜੇ (font->slant)
      angle = hb_ਅਨੁਪਾਤ_ਮੰਜ਼ਿਲ_angle (font->slant + hb_angle_ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ (angle));

    ਵਾਪਸ angle;
  }
  ਹਾਲਤ HB_STYLE_ਟੈਗ_ਚੌੜਾਈ:
    ਵਾਪਸ face->ਸਾਰਣੀ.OS2->has_ਅੰਕੜੇ ()
	   ? face->ਸਾਰਣੀ.OS2->get_ਚੌੜਾਈ ()
	   : (face->ਸਾਰਣੀ.ਸਿਰ_2->is_condensed () ? 75 :
	      face->ਸਾਰਣੀ.ਸਿਰ_2->is_expanded () ? 125 :
	      100);
  ਹਾਲਤ HB_STYLE_ਟੈਗ_ਭਾਰ:
    ਵਾਪਸ face->ਸਾਰਣੀ.OS2->has_ਅੰਕੜੇ ()
	   ? face->ਸਾਰਣੀ.OS2->us_ਭਾਰ_Class
	   : (face->ਸਾਰਣੀ.ਸਿਰ_2->is_bold () ? 700 : 400);
  ਮੂਲ:
    ਵਾਪਸ 0;
  }
}

#ਜੇ_ਅੰਤ
