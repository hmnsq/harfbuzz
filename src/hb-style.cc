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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_STYLE

#شامل "hb-ot-var-avar-table.hh"
#شامل "hb-ot-var-fvar-table.hh"
#شامل "hb-ot-stat-table.hh"
#شامل "hb-ot-os2-table.hh"
#شامل "hb-ot-head-table.hh"
#شامل "hb-ot-post-table.hh"
#شامل "hb-ot-face.hh"

/**
 * SECTION:hb-style
 * @title: hb-style
 * @short_description: Font Styles
 * @include: hb.h
 *
 * Functions for fetching style information from fonts.
 **/

ساکن اندرونی اعشاری
hb_angle_مقصد_تناسب (اعشاری a)
{
  واپس tanf (a * -HB_PI / 180.f);
}

ساکن اندرونی اعشاری
hb_تناسب_مقصد_angle (اعشاری r)
{
  واپس atanf (r) * -180.f / HB_PI;
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
اعشاری
hb_style_get_قدر (hb_font_t *font, hb_style_لیبل_t style_لیبل)
{
  اگر (unlikely (style_لیبل == HB_STYLE_لیبل_SLANT_تناسب))
    واپس hb_angle_مقصد_تناسب (hb_style_get_قدر (font, HB_STYLE_لیبل_SLANT_ANGLE));

  hb_face_t *face = font->face;

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  hb_ot_var_axis_info_t axis;
  اگر (hb_ot_var_ڈھونڈو_axis_info (face, style_لیبل, &axis))
  {
    اگر (axis.axis_اشاریہ < font->عدد_coords) واپس font->design_coords[axis.axis_اشاریہ];
    /* If a face is variable, fvar's default_value is better than STAT records */
    واپس axis.default_قدر;
  }
#اگر_ختم

  اگر (style_لیبل == HB_STYLE_لیبل_OPTICAL_سائز && font->ptem)
    واپس font->ptem;

  /* STAT */
  اعشاری value;
  اگر (face->جدول.STAT->get_قدر (style_لیبل, &value))
    واپس value;

  انتخاب ((غیرعلامتی) style_لیبل)
  {
  حالت HB_STYLE_لیبل_ITALIC:
    واپس face->جدول.OS2->is_italic () || face->جدول.سر_2->is_italic () ? 1 : 0;
  حالت HB_STYLE_لیبل_OPTICAL_سائز:
  {
    غیرعلامتی صحیح زیریں, design, بالائی;
    واپس face->جدول.OS2->v5 ().get_optical_سائز (&زیریں, &بالائی)
	   ? (اعشاری) (زیریں + بالائی) / 2.f
	   : hb_ot_layout_get_سائز_params (face, &design, خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ)
	   ? design / 10.f
	   : 12.f;
  }
  حالت HB_STYLE_لیبل_SLANT_ANGLE:
  {
    اعشاری angle = face->جدول.post->جدول->italicAngle.مقصد_float ();

    اگر (font->slant)
      angle = hb_تناسب_مقصد_angle (font->slant + hb_angle_مقصد_تناسب (angle));

    واپس angle;
  }
  حالت HB_STYLE_لیبل_چوڑائی:
    واپس face->جدول.OS2->has_ڈیٹا ()
	   ? face->جدول.OS2->get_چوڑائی ()
	   : (face->جدول.سر_2->is_condensed () ? 75 :
	      face->جدول.سر_2->is_expanded () ? 125 :
	      100);
  حالت HB_STYLE_لیبل_وزن:
    واپس face->جدول.OS2->has_ڈیٹا ()
	   ? face->جدول.OS2->us_وزن_Class
	   : (face->جدول.سر_2->is_bold () ? 700 : 400);
  طےشدہ:
    واپس 0;
  }
}

#اگر_ختم
