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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_STYLE

#અન્તર્ભાવ "hb-ot-var-avar-table.hh"
#અન્તર્ભાવ "hb-ot-var-fvar-table.hh"
#અન્તર્ભાવ "hb-ot-stat-table.hh"
#અન્તર્ભાવ "hb-ot-os2-table.hh"
#અન્તર્ભાવ "hb-ot-head-table.hh"
#અન્તર્ભાવ "hb-ot-post-table.hh"
#અન્તર્ભાવ "hb-ot-face.hh"

/**
 * SECTION:hb-style
 * @title: hb-style
 * @short_description: Font Styles
 * @include: hb.h
 *
 * Functions for fetching style information from fonts.
 **/

સ્થિર અંતઃરેખ દશાંશ
_hb_angle_to_ratio (દશાંશ a)
{
  પાછા tanf (a * -HB_PI / 180.f);
}

સ્થિર અંતઃરેખ દશાંશ
_hb_ratio_to_angle (દશાંશ r)
{
  પાછા atanf (r) * -180.f / HB_PI;
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
દશાંશ
hb_style_get_કિંમત (hb_font_t *font, hb_style_tag_t style_tag)
{
  જો (unlikely (style_tag == HB_STYLE_TAG_SLANT_RATIO))
    પાછા _hb_angle_to_ratio (hb_style_get_કિંમત (font, HB_STYLE_TAG_SLANT_ANGLE));

  hb_face_t *face = font->face;

#ifndef HB_NO_VAR
  hb_ot_var_axis_info_t axis;
  જો (hb_ot_var_find_axis_info (face, style_tag, &axis))
  {
    જો (axis.axis_index < font->અંક_coords) પાછા font->design_coords[axis.axis_index];
    /* If a face is variable, fvar's default_value is better than STAT records */
    પાછા axis.default_કિંમત;
  }
#endif

  જો (style_tag == HB_STYLE_TAG_OPTICAL_SIZE && font->ptem)
    પાછા font->ptem;

  /* STAT */
  દશાંશ value;
  જો (face->table.STAT->get_કિંમત (style_tag, &value))
    પાછા value;

  પસંદગી ((unsigned) style_tag)
  {
  સ્થિતિ HB_STYLE_TAG_ITALIC:
    પાછા face->table.OS2->is_italic () || face->table.head->is_italic () ? 1 : 0;
  સ્થિતિ HB_STYLE_TAG_OPTICAL_SIZE:
  {
    unsigned પૂર્ણાંક lower, design, upper;
    પાછા face->table.OS2->v5 ().get_optical_size (&lower, &upper)
	   ? (દશાંશ) (lower + upper) / 2.f
	   : hb_ot_layout_get_size_params (face, &design, nullptr, nullptr, nullptr, nullptr)
	   ? design / 10.f
	   : 12.f;
  }
  સ્થિતિ HB_STYLE_TAG_SLANT_ANGLE:
  {
    દશાંશ angle = face->table.post->table->italicAngle.to_float ();

    જો (font->slant)
      angle = _hb_ratio_to_angle (font->slant + _hb_angle_to_ratio (angle));

    પાછા angle;
  }
  સ્થિતિ HB_STYLE_TAG_WIDTH:
    પાછા face->table.OS2->has_data ()
	   ? face->table.OS2->get_width ()
	   : (face->table.head->is_condensed () ? 75 :
	      face->table.head->is_expanded () ? 125 :
	      100);
  સ્થિતિ HB_STYLE_TAG_WEIGHT:
    પાછા face->table.OS2->has_data ()
	   ? face->table.OS2->usWeightClass
	   : (face->table.head->is_bold () ? 700 : 400);
  મૂળભૂત:
    પાછા 0;
  }
}

#endif
