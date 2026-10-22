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

#ضمن "hb.hh"

#جيڪڏهن_وصف_نه_ٿيل HB_NO_STYLE

#ضمن "hb-ot-var-avar-table.hh"
#ضمن "hb-ot-var-fvar-table.hh"
#ضمن "hb-ot-stat-table.hh"
#ضمن "hb-ot-os2-table.hh"
#ضمن "hb-ot-head-table.hh"
#ضمن "hb-ot-post-table.hh"
#ضمن "hb-ot-face.hh"

/**
 * SECTION:hb-style
 * @title: hb-style
 * @short_description: Font Styles
 * @include: hb.h
 *
 * Functions for fetching style information from fonts.
 **/

ساكن مضمن عائم
hb_angle_منزل_تناسب (عائم a)
{
  أرجع tanf (a * -HB_PI / 180.f);
}

ساكن مضمن عائم
hb_تناسب_منزل_angle (عائم r)
{
  أرجع atanf (r) * -180.f / HB_PI;
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
عائم
hb_style_get_قدر (hb_font_t *font, hb_style_ٽيگ_t style_ٽيگ)
{
  إذا (unlikely (style_ٽيگ == HB_STYLE_ٽيگ_SLANT_تناسب))
    أرجع hb_angle_منزل_تناسب (hb_style_get_قدر (font, HB_STYLE_ٽيگ_SLANT_ANGLE));

  hb_face_t *face = font->face;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  hb_ot_var_axis_info_t axis;
  إذا (hb_ot_var_ڳوليو_axis_info (face, style_ٽيگ, &axis))
  {
    إذا (axis.axis_اشاريو < font->انگ_coords) أرجع font->design_coords[axis.axis_اشاريو];
    /* If a face is variable, fvar's default_value is better than STAT records */
    أرجع axis.default_قدر;
  }
#جيڪڏهن_پورو

  إذا (style_ٽيگ == HB_STYLE_ٽيگ_OPTICAL_سائيز && font->ptem)
    أرجع font->ptem;

  /* STAT */
  عائم value;
  إذا (face->جدول.STAT->get_قدر (style_ٽيگ, &value))
    أرجع value;

  بدل ((بنانشان) style_ٽيگ)
  {
  حالة HB_STYLE_ٽيگ_ITALIC:
    أرجع face->جدول.OS2->is_italic () || face->جدول.سر_2->is_italic () ? 1 : 0;
  حالة HB_STYLE_ٽيگ_OPTICAL_سائيز:
  {
    بنانشان صحيح هيٺاهون, design, مٿانهون;
    أرجع face->جدول.OS2->v5 ().get_optical_سائيز (&هيٺاهون, &مٿانهون)
	   ? (عائم) (هيٺاهون + مٿانهون) / 2.f
	   : hb_ot_layout_get_سائيز_params (face, &design, خالي_اشارو, خالي_اشارو, خالي_اشارو, خالي_اشارو)
	   ? design / 10.f
	   : 12.f;
  }
  حالة HB_STYLE_ٽيگ_SLANT_ANGLE:
  {
    عائم angle = face->جدول.post->جدول->italicAngle.منزل_float ();

    إذا (font->slant)
      angle = hb_تناسب_منزل_angle (font->slant + hb_angle_منزل_تناسب (angle));

    أرجع angle;
  }
  حالة HB_STYLE_ٽيگ_ويڪر:
    أرجع face->جدول.OS2->has_معلومات ()
	   ? face->جدول.OS2->get_ويڪر ()
	   : (face->جدول.سر_2->is_condensed () ? 75 :
	      face->جدول.سر_2->is_expanded () ? 125 :
	      100);
  حالة HB_STYLE_ٽيگ_وزن:
    أرجع face->جدول.OS2->has_معلومات ()
	   ? face->جدول.OS2->us_وزن_Class
	   : (face->جدول.سر_2->is_bold () ? 700 : 400);
  افتراضي:
    أرجع 0;
  }
}

#جيڪڏهن_پورو
